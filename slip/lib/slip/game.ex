defmodule Slip.Game do
  use GenServer
  use Bitwise
  alias Slip.Link
  alias Slip.Scores
  alias Phoenix.PubSub

  def start_link(_opts \\ []) do
    GenServer.start_link __MODULE__, :ok, name: __MODULE__
  end

  def get_state do
    GenServer.call __MODULE__, :get_state
  end

  def get_menu do
    GenServer.call __MODULE__, :get_menu
  end

  def get_game do
    GenServer.call __MODULE__, :get_game
  end

  def set_player(player) do
    GenServer.cast __MODULE__, {:set_player, player}
  end

  def register(message) do
    GenServer.cast __MODULE__, message
  end


  def init(:ok) do
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
          items: Scores.list_players()
        }
      },

      %{
        title: "Reset",
        selected: false,
        sub_menu: %{
          title: "Reset what?",
          items: [
            %{
              title: "Controller",
              selected: true,
              effect: fn -> nil
              end
            },
            %{
              title: "Scores",
              selected: false,
              effect: fn -> nil
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
              effect: fn -> nil
              end
            },
            %{
              title: "Bend Ova",
              selected: false,
              effect: fn -> nil
              end
            }
          ]
        }
      }
    ]

    state = %{
      menu: menu,
      sub_menu: hd(menu).sub_menu,
      in_main_menu: true,
      show: :menu,
      player: nil,
      score: 0
    }

    {:ok, state}
  end

  def handle_call(:get_state, _from, state) do
    {:reply, state.show, state}
  end

  def handle_call(:get_menu, _from, state) do
    %{
      menu: menu,
      sub_menu: sub_menu,
      in_main_menu: in_main?
    } = state

    {:reply, {menu, sub_menu, in_main?}, state}
  end

  def handle_call(:get_game, _from, %{player: p, score: s} =  state) do
    {:reply, {p, s}, state}
  end

  def handle_cast({:set_player, player}, state) do
    PubSub.broadcast Slip.PubSub, "slip", {:player, player}
    {:noreply, %{state | player: player, score: 0}}
  end

  def handle_cast({:joystick_lp, :left}, %{show: :menu} = state) do
    new_state = %{state | in_main_menu: true}
    PubSub.broadcast Slip.PubSub, "slip", {:new, new_state}
    {:noreply, new_state}
  end

  def handle_cast({:joystick_lp, :right}, %{show: :menu} = state) do
    new_state = change_menu_or_activate(state)
    PubSub.broadcast Slip.PubSub, "slip", {:new, new_state}
    {:noreply, new_state}
  end

  def handle_cast({:joystick_lp, :up}, %{show: :menu} = state) do
    new_state = select_prev_menu_item(state)
    PubSub.broadcast Slip.PubSub, "slip", {:new, new_state}
    {:noreply, new_state}
  end

  def handle_cast({:joystick_lp, :down}, %{show: :menu} = state) do
    new_state = select_next_menu_item(state)
    PubSub.broadcast Slip.PubSub, "slip", {:new, new_state}
    {:noreply, new_state}
  end

  def handle_cast({:can, origin, can}, state) do
    PubSub.broadcast Slip.PubSub, "slip", {:can, origin, can}
    {:noreply, state}
  end

  def handle_cast({:score_low, byte}, state) do
    {:noreply, %{state | score: state.score ||| byte}}
  end

  def handle_cast({:score_high, byte}, state) do
    {:noreply, %{state | score: state.score ||| (byte <<< 8)}}
  end

  def handle_cast({:show, show}, state) do
    PubSub.broadcast Slip.PubSub, "slip", {:show, show}
    {:noreply, %{state | show: show}}
  end

  def handle_cast(_, state) do
    {:noreply, state}
  end


  # Helper functions

  defp select_next_menu_item(state) do
    if state.in_main_menu do
      menu = select_next [], state.menu
      sub_menu = fetch_sub_menu menu
      state
      |> Map.put(:menu, menu)
      |> Map.put(:sub_menu, sub_menu)
    else
      menu = state.sub_menu
      items = select_next [], menu.items
      Map.put state, :sub_menu, %{menu | items: items}
    end
  end

  defp select_prev_menu_item(state) do
    if state.in_main_menu do
      menu = select_next [], Enum.reverse(state.menu)
      sub_menu = fetch_sub_menu menu
      state
      |> Map.put(:menu, Enum.reverse(menu))
      |> Map.put(:sub_menu, sub_menu)
    else
      menu = state.sub_menu
      items = select_next [], Enum.reverse(menu.items)
      Map.put state, :sub_menu, %{menu | items: Enum.reverse(items)}
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

  defp change_menu_or_activate(state) do
    if state.in_main_menu do
      Map.put(state, :in_main_menu, false)
    else
      effect = state
        |> Map.fetch!(:sub_menu)
        |> Map.fetch!(:items)
        |> Enum.filter(&(&1.selected))
        |> hd()
        |> Map.fetch!(:effect)
      effect.()
      state
    end
  end
end
