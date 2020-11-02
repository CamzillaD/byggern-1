defmodule SlipWeb.PageLive do
  use SlipWeb, :live_view
  alias Slip.Link
  alias Phoenix.PubSub

  def mount(_params, _session, socket) do
    if connected?(socket) do
      PubSub.subscribe(Slip.PubSub, "slip")
    end

    menu = [
      %{
        title: "Indicator",
        selected: true,
        sub_menu: %{
          title: "Turn on or off?",
          items: [
            %{
              title: "On",
              selected: true,
              effect: fn ->
                Link.set_indicator(true)
              end
            },
            %{
              title: "Off",
              selected: false,
              effect: fn ->
                Link.set_indicator(false)
              end
            }
          ]
        }
      },

      %{
        title: "Play Game",
        selected: false,
        sub_menu: %{
          title: "Play as:",
          items: [
            %{
              title: "Kolbjørn",
              selected: true,
              effect: fn ->
              end
            },
            %{
              title: "Camilla",
              selected: false,
              effect: fn ->
              end
            },
            %{
              title: "Jon",
              selected: false,
              effect: fn ->
              end
            },
          ]
        }
      },

      %{
        title: "Reset",
        selected: false,
        sub_menu: %{
          title: "Reset what?",
          items: [
            %{
              title: "Scores",
              selected: true,
              effect: fn ->
              end
            },
            %{
              title: "Controller",
              selected: false,
              effect: fn ->
              end
            }
          ]
        }
      },

      %{
        title: "Play Song",
        selected: false,
        sub_menu: %{
          title: "Which song?",
          items: [
            %{
              title: "Wet Ass Pussy",
              selected: true,
              effect: fn ->
              end
            },
            %{
              title: "Bend Ova",
              selected: false,
              effect: fn ->
              end
            }
          ]
        }
      }
    ]

    new_socket = assign(socket,
      menu: menu,
      sub_menu: hd(menu).sub_menu,
      in_main_menu: true,
      unhandled: 0,
    )

    {:ok, new_socket}
  end

  def handle_event("toggle-light", _params, socket) do
    on? = !socket.assigns.indicator_on
    Link.set_indicator on?
    {:noreply, assign(socket, indicator_on: on?)}
  end

  def handle_event("reset", _params, socket) do
    {:noreply, assign(socket, :unhandled, 0)}
  end

  def handle_info({:joystick_lp, move}, socket) do
    case move do
      :left ->
        {:noreply, assign(socket, :in_main_menu, true)}

      :right ->
        {:noreply, change_menu_or_activate(socket)}

      :up ->
        {:noreply, select_prev_menu_item(socket)}

      :down ->
        {:noreply, select_next_menu_item(socket)}

      _ ->
        {:noreply, socket}
    end
  end


  defp change_menu_or_activate(socket) do
    if socket.assigns.in_main_menu do
      assign(socket, :in_main_menu, false)
    else
      effect = socket.assigns.sub_menu.items
        |> Enum.filter(&(&1.selected))
        |> hd()
        |> Map.fetch!(:effect)
      effect.()
      socket
    end
  end



  def handle_info(_, socket) do
    {:noreply, update(socket, :unhandled, &(&1 + 1))}
  end


  # Helper functions

  defp select_next_menu_item(socket) do
    if socket.assigns.in_main_menu do
      menu = select_next [], socket.assigns.menu
      sub_menu = fetch_sub_menu menu
      assign(socket, menu: menu, sub_menu: sub_menu)
    else
      menu = socket.assigns.sub_menu
      items = select_next [], menu.items
      assign(socket, sub_menu: %{menu | items: items})
    end
  end

  defp select_prev_menu_item(socket) do
    if socket.assigns.in_main_menu do
      menu = select_next [], Enum.reverse(socket.assigns.menu)
      sub_menu = fetch_sub_menu menu
      assign(socket, menu: Enum.reverse(menu), sub_menu: sub_menu)
    else
      menu = socket.assigns.sub_menu
      items = select_next [], Enum.reverse(menu.items)
      assign(socket, sub_menu: %{menu | items: Enum.reverse(items)})
    end
  end

  defp select_next(before, [%{selected: true} = head | tail]) do
    case tail do
      [] ->
        before ++ [head]

      [next | rest] ->
        before
        ++ [%{head | selected: false}, %{next | selected: true}]
        ++ rest
    end
  end

  defp select_next(before, [head | tail]) do
    select_next before ++ [head], tail
  end

  defp fetch_sub_menu(menu) do
    menu
    |> Enum.filter(&(&1.selected))
    |> hd()
    |> Map.fetch!(:sub_menu)
  end
end
