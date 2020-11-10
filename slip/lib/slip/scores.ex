defmodule Slip.Scores do

  def list_players do
    [
      "Kolbjørn",
      "Leonie",
      "Ernesto",
      "Camilla",
      "Jon"
    ]
    |> Enum.map(&build_map/1)
    |> select_first([])
  end

  defp select_first([], rest) do
    Enum.reverse rest
  end

  defp select_first([head | tail], []) do
    select_first tail, [Map.put(head, :selected, true)]
  end

  defp select_first([head | tail], rest) do
    select_first tail, [Map.put(head, :selected, false) | rest]
  end

  defp build_map(name) do
    %{
      title: name,
      effect: fn ->
        Slip.Game.set_player(name)
        Slip.Link.start_game()
      end
    }
  end
end
