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
        title: "Play Game",
        selected: true,
        sub_title: "Play as:",
        secondary: [
          %{
            title: "Kolbjørn",
            selected: true
          },
          %{
            title: "Camilla",
            selected: false
          },
          %{
            title: "Jon",
            selected: false
          },
        ]
      },

      %{
        title: "Reset",
        selected: false,
        sub_title: "Reset what?",
        secondary: [
          %{
            title: "Scores",
            selected: true
          },
          %{
            title: "Controller",
            selected: false
          }
        ]
      },

      %{
        title: "Play Song",
        selected: false,
        sub_title: "Which song?",
        secondary: [
          %{
            title: "Wet Ass Pussy",
            selected: true
          },
          %{
            title: "Bend Ova",
            selected: false
          }
        ]
      }
    ]

    {:ok, assign(socket, menu: menu, in_main: true, unhandled: 0)}
  end

  def handle_event("toggle-light", _params, socket) do
    on? = !socket.assigns.indicator_on
    Link.set_indicator on?
    {:noreply, assign(socket, indicator_on: on?)}
  end

  def handle_event("reset", _params, socket) do
    {:noreply, assign(socket, :unhandled, 0)}
  end

  # def handle_event("menu", %{"item" => item}, socket) do
  #   menu = socket.assigns.menu
  #     |> Enum.map(&(Map.put &1, :selected, false))
  #     |> Enum.map(fn m ->
  #       if m.title == item do
  #         Map.put(m, :selected, true)
  #       else
  #         m
  #       end
  #     end)
  #   {:noreply, assign(socket, :menu, menu)}
  # end

  def handle_info({:joystick_lp, move}, socket) do
    case move do
      :left ->
        {:noreply, assign(socket, :in_main, true)}

      :right ->
        {:noreply, assign(socket, :in_main, false)}

      :up ->
        {:noreply, update(socket, :menu, &select_prev/1)}

      :down ->
        {:noreply, update(socket, :menu, &select_next/1)}

      _ ->
        {:noreply, socket}
    end
  end


  defp select_next(menu) do
    select_next [], menu
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

  defp select_prev(menu) do
    menu
    |> Enum.reverse()
    |> select_next()
    |> Enum.reverse()
  end




  def handle_info({:joystick_lh, raw}, socket) do
    value = round(raw * -70 / 247 + 85)
    {:noreply, assign(socket, :joystick_lh, value)}
  end

  def handle_info({:joystick_lv, raw}, socket) do
    value = round(raw * -70 / 247 + 85)
    {:noreply, assign(socket, :joystick_lv, value)}
  end

  def handle_info(_, socket) do
    {:noreply, update(socket, :unhandled, &(&1 + 1))}
  end
end
