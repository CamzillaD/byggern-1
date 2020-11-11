defmodule Slip.Scores do

  def list_players do
    read_scores_file()
    |> select_first([])
  end

  def list_scores do
    read_scores_file()
    |> Enum.map(&(Map.put &1, :effect, fn -> nil end))
    |> Enum.map(fn entry ->
      name = entry.title
      score = entry.score
      %{entry | title: "#{name} (#{score})"}
    end)
    |> select_first([])
  end

  def persist_score(player, score) do
    all_players = read_scores_file()

    updated_players = Enum.map all_players, fn p ->
      if p.title == player && score > p.score do
        Map.put p, :score, score
      else
        p
      end
    end

    encoding = Enum.reduce updated_players, "", fn p, acc ->
      acc <> "#{p.title},#{p.score}\n"
    end

    File.write! "scores", encoding
  end

  def clear_scores do
    encoding = read_scores_file()
    |> Enum.reduce("", fn p, acc ->
      acc <> "#{p.title},0\n"
    end)

    File.write! "scores", encoding
  end

  defp read_scores_file do
    "scores"
    |> File.read!()
    |> String.split("\n", trim: true)
    |> Enum.map(fn pair ->
      [name, score_str] = String.split pair, ",", trim: true
      {score, _} = Integer.parse score_str
      %{
        title: name,
        score: score,
        effect: fn ->
          Slip.Game.set_player(name)
          Slip.Link.start_game()
        end
      }
    end)
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
