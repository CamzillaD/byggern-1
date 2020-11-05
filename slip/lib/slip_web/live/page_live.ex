defmodule SlipWeb.PageLive do
  use SlipWeb, :live_view
  alias Slip.Game
  alias Phoenix.PubSub

  def mount(_params, _session, socket) do
    if connected?(socket) do
      PubSub.subscribe(Slip.PubSub, "slip")
    end

    {menu, sub_menu, in_main?} = Game.get_menu()

    new_socket = assign(socket,
      menu: menu,
      sub_menu: sub_menu,
      in_main_menu: in_main?
    )

    {:ok, new_socket}
  end

  def handle_info(new_state, socket) do
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
end
