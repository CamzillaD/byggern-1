defmodule SlipWeb.PageLive do
  use SlipWeb, :live_view
  alias Slip.Game
  alias Phoenix.PubSub

  def mount(_params, _session, socket) do
    if connected?(socket) do
      PubSub.subscribe(Slip.PubSub, "slip")
    end

    state = Game.get_state()
    {menu, sub_menu, in_main?} = Game.get_menu()
    {player, score} = Game.get_game()

    new_socket = assign(socket,
      state: state,
      menu: menu,
      sub_menu: sub_menu,
      in_main_menu: in_main?,
      player: player,
      score: score,
      defer_menu: false
    )

    {:ok, new_socket}
  end

  def handle_info({:new, new_state}, socket) do
    %{
      menu: menu,
      sub_menu: sub_menu,
      in_main_menu: in_main?
    } = new_state

    new_socket = socket
      |> assign(:menu, menu)
      |> assign(:sub_menu, sub_menu)
      |> assign(:in_main_menu, in_main?)

    {:noreply, new_socket}
  end

  def handle_info({:show, :menu}, socket) do
    unless socket.assigns.defer_menu do
      {:noreply, assign(socket, :state, :menu)}
      else
      {:noreply, socket}
    end
  end

  def handle_info({:show, :game}, socket) do
    {:noreply, assign(socket, :state, :game)}
  end

  def handle_info({:show, :score}, socket) do
    :timer.send_after 3000, self(), :return_to_menu

    new_socket = socket
      |> assign(:state, :score)
      |> assign(:defer_menu, true)

    {:noreply, new_socket}
  end

  def handle_info(:return_to_menu, socket) do
    new_socket = socket
      |> assign(:state, :menu)
      |> assign(:defer_menu, false)

    {:noreply, new_socket}
  end

  def handle_info({:player, player}, socket) do
    {:noreply, assign(socket, :player, player)}
  end

  def handle_info({:score, score}, socket) do
    {:noreply, assign(socket, :score, score)}
  end

  def handle_info(_, socket) do
    {:noreply, socket}
  end
end
