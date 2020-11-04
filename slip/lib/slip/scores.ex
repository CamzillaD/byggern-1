defmodule Slip.Scores do

  def list_players do
    [
      "Kolbjørn",
      "Ernesto",
      "Camilla",
      "Jon"
    ]
    |> Enum.map(&(%{title: &1, effect: fn -> end}))
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
end
