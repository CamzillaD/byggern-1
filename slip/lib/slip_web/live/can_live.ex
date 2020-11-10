defmodule SlipWeb.CanLive do
  use SlipWeb, :live_view
  alias Phoenix.PubSub
  alias Slip.Link

  def mount(_params, _session, socket) do
    if connected?(socket) do
      PubSub.subscribe(Slip.PubSub, "slip")
    end

    new_socket = assign(socket,
      can_messages_tx: 0,
      last_tx: [],
      can_messages_rx: 0,
      last_rx: []
    )

    {:ok, new_socket}
  end

  def handle_event("reset", _params, socket) do
    Link.request_slave_reset()

    new_socket = socket
      |> assign(:can_messages_tx, 0)
      |> assign(:last_tx, [])
      |> assign(:can_messages_rx, 0)
      |> assign(:last_rx, [])

    {:noreply, new_socket}
  end

  def handle_info({:can, 1, can}, socket) do
    new_socket = socket
      |> update(:can_messages_tx, &(&1 + 1))
      |> update(:last_tx, &(add_can_message &1, can))
    {:noreply, new_socket}
  end

  def handle_info({:can, 0, can}, socket) do
    new_socket = socket
      |> update(:can_messages_rx, &(&1 + 1))
      |> update(:last_rx, &(add_can_message &1, can))
    {:noreply, new_socket}
  end

  def handle_info(_, socket) do
    {:noreply, socket}
  end

  defp add_can_message(messages, new) do
    [new | messages]
    |> Enum.take(2)
  end
end
