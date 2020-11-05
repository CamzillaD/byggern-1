defmodule Slip.Link do
  use GenServer
  use Bitwise
  alias Circuits.UART
  alias Slip.Game

  @sof_flag 0x02

  @event_indicator 0x16
  @event_request_reset 0x17

  @event_joystick_lh 0x31
  @event_joystick_lv 0x32
  @event_joystick_lp 0x33
  # @event_joystick_ls 0x34

  @event_joystick_rh 0x36
  @event_joystick_rv 0x37
  @event_joystick_rp 0x38
  # @event_joystick_rs 0x39

  @event_can_clear   0x40
  @event_can_id_low  0x42
  @event_can_id_high 0x43
  @event_can_size    0x44
  @event_can_data    0x45
  @event_can_commit  0x4f

  @event_can_interrupt 0x66

  @event_generic 0xef


  def start_link(_opts \\ []) do
    GenServer.start_link __MODULE__, :ok, name: __MODULE__
  end

  def set_indicator(on?) do
    GenServer.call __MODULE__, {:indicator, on?}
  end

  def request_slave_reset do
    GenServer.call __MODULE__, :request_reset
  end

  def can_send(can_message) do
    GenServer.call __MODULE__, {:can_send, can_message}
  end


  def init(:ok) do
    uart_port = Application.get_env :slip, :uart_port, "ttyACM0"
    uart_baud = Application.get_env :slip, :uart_baud, 9600

    {:ok, pid} = UART.start_link()
    UART.open pid, uart_port, speed: uart_baud, active: true

    state = %{
      buffer: <<>>,
      uart: pid,
      can: %{
        id: 0,
        size: 0,
        data: []
      }
    }

    {:ok, state}
  end

  def handle_call({:indicator, on?}, _from, %{uart: uart} = state) do
    value = if on?, do: 0x01, else: 0x00
    write uart, @event_indicator, value
    {:reply, :ok, state}
  end

  def handle_call(:request_reset, _from, %{uart: uart} = state) do
    write uart, @event_request_reset, 0x00
    {:reply, :ok, state}
  end

  def handle_call({:can_send, can}, _from, %{uart: uart} = state) do
    valid_id = can.id >= 0 && can.id < 2048
    valid_size = can.size >= 0 && can.size <= 8

    if (valid_id && valid_size) do
      write uart, @event_can_clear, 0x01
      write uart, @event_can_id_low, can.id &&& 0xff
      write uart, @event_can_id_high, (can.id >>> 8) &&& 0xff
      write uart, @event_can_size, can.size

      Enum.each can.data, fn d ->
        write uart, @event_can_data, d &&& 0xff
      end

      write uart, @event_can_commit, 0x01
      {:reply, :ok, state}
    else
      {:reply, :error, state}
    end
  end

  def handle_info({:circuits_uart, _port, r}, %{buffer: b} = state) do
    {new_buffer, messages} = parse_buffer b <> r

    new_can = Enum.reduce messages, state.can, fn m, can ->
      register(m, can)
    end

    new_state = %{state | buffer: new_buffer, can: new_can}
    {:noreply, new_state}
  end

  defp parse_buffer(buffer) do
    parse_buffer buffer, []
  end

  defp parse_buffer(<<head :: binary-size(4), tail :: binary>>, parsed) do
    {remainder, message} = parse_message head
    parse_buffer remainder <> tail, [message | parsed]
  end

  defp parse_buffer(less_than_four_bytes, parsed) do
    {less_than_four_bytes, parsed}
  end

  defp parse_message(<<@sof_flag, event, value, sum>>) do
    case (event + value) &&& 0xff do
      ^sum ->
        {<<>>, {event, value}}

      _ ->
        {<<>>, {}}
    end
  end

  defp parse_message(<<_not_sof :: binary-size(1), tail :: binary>>) do
    {tail, {}}
  end

  # Messages from slave to host

  defp register({event, value}, can) do
    {send?, message, new_can} = case event do
      @event_joystick_lh ->
        {true, {:joystick_lh, value}, can}

      @event_joystick_lv ->
        {true, {:joystick_lv, value}, can}

      @event_joystick_lp ->
        {true, {:joystick_lp, joystick_position(value)}, can}



      @event_joystick_rh ->
        {true, {:joystick_rh, value}, can}

      @event_joystick_rv ->
        {true, {:joystick_rv, value}, can}

      @event_joystick_rp ->
        {true, {:joystick_rp, joystick_position(value)}, can}


      @event_can_clear ->
        {false, nil, %{can | data: [], id: 0}}

      @event_can_id_low ->
        {false, nil, %{can | id: value ||| can.id}}

      @event_can_id_high ->
        {false, nil, %{can | id: (value <<< 8) ||| can.id}}

      @event_can_size ->
        {false, nil, %{can | size: value}}

      @event_can_data ->
        {false, nil, %{can | data: [value | can.data]}}

      @event_can_commit ->
        {true, {:can, %{can | data: Enum.reverse(can.data)}}, can}


      @event_can_interrupt ->
        {true, :can_interrupt, can}

      @event_generic ->
        {true, {:generic, value}, can}

      _ ->
        {false, :unknown, can}
    end

    if send? do
      Game.register message
    end

    new_can
  end

  defp register(_, can) do
    can
  end

  defp joystick_position(1), do: :center
  defp joystick_position(2), do: :right
  defp joystick_position(3), do: :left
  defp joystick_position(4), do: :up
  defp joystick_position(5), do: :down
  defp joystick_position(_), do: :invalid

  # Messages from host to slave

  defp write(uart, event, value) do
    message = <<@sof_flag, event, value, (event + value) &&& 0xfff>>
    UART.write uart, message
  end

end
