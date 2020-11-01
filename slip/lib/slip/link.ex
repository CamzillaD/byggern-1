defmodule Slip.Link do
  use GenServer
  use Bitwise
  alias Circuits.UART

  @sof_flag 0x02

  @event_indicator 0x16

  @event_joystick_lh 0x31
  @event_joystick_lv 0x32
  @event_joystick_lp 0x33
  @event_joystick_ls 0x34

  @event_joystick_rh 0x36
  @event_joystick_rv 0x37
  @event_joystick_rp 0x38
  @event_joystick_rs 0x39


  def start_link(_opts \\ []) do
    GenServer.start_link __MODULE__, :ok, name: __MODULE__
  end

  def set_indicator(on?) do
    GenServer.call __MODULE__, {:indicator, on?}
  end


  def init(:ok) do
    uart_port = Application.get_env :slip, :uart_port, "ttyACM0"
    uart_baud = Application.get_env :slip, :uart_baud, 9600

    {:ok, pid} = UART.start_link()
    UART.open pid, uart_port, speed: uart_baud, active: true

    state = %{
      buffer: <<>>,
      uart: pid
    }

    {:ok, state}
  end

  def handle_call({:indicator, on?}, _from, %{uart: uart} = state) do
    value = if on?, do: 0x01, else: 0x00
    write uart, @event_indicator, value
    {:reply, :ok, state}
  end

  def handle_info({:circuits_uart, _port, r}, %{buffer: b} = state) do
    {new_buffer, messages} = parse_buffer b <> r

    Enum.each messages, fn m ->
      broadcast(m)
    end

    new_state = %{state | buffer: new_buffer}
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

  defp broadcast({event, value}) do
    message = case event do
      @event_joystick_lh ->
        {:joystick_lh, value}

      @event_joystick_lv ->
        {:joystick_lv, value}

      @event_joystick_lp ->
        {:joystick_lp, joystick_position(value)}



      @event_joystick_rh ->
        {:joystick_rh, value}

      @event_joystick_rv ->
        {:joystick_rv, value}

      @event_joystick_rp ->
        {:joystick_rp, joystick_position(value)}

      _ ->
        :unknown
    end

    Phoenix.PubSub.broadcast Slip.PubSub, "slip", message
  end

  defp broadcast(_) do
    Phoenix.PubSub.broadcast Slip.PubSub, "slip", :unknown
  end

  defp joystick_position(1), do: :center
  defp joystick_position(2), do: :left
  defp joystick_position(3), do: :right
  defp joystick_position(4), do: :up
  defp joystick_position(5), do: :down
  defp joystick_position(_), do: :invalid

  # Messages from host to slave

  defp write(uart, event, value) do
    message = <<@sof_flag, event, value, (event + value) &&& 0xfff>>
    UART.write uart, message
  end

end
