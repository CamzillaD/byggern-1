defmodule SlipWeb.CanLive do
  use SlipWeb, :live_view
  alias Phoenix.PubSub

  def mount(_params, _session, socket) do
    if connected?(socket) do
      PubSub.subscribe(Slip.PubSub, "slip")
    end

    new_socket = assign(socket,
      can_messages_received: 0,
      can_messages_sent: 0,
      last_can_messages: []
    )

    {:ok, new_socket}
  end

  def handle_info({:can, can}, socket) do
    new_socket = socket
      |> update(:can_messages_received, &(&1 + 1))
      |> update(:last_can_messages, &(add_can_message &1, can))
    {:noreply, new_socket}
  end

  def handle_info(_, socket) do
    {:noreply, socket}
  end

  defp add_can_message(messages, new) do
    [new | messages]
    |> Enum.take(3)
  end
end
