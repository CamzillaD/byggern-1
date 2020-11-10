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
      score: score
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

  def handle_info({:show, state}, socket) do
    {:noreply, assign(socket, :state, state)}
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
