defmodule SlipWeb.PageLive do
  use SlipWeb, :live_view
  alias Slip.Link
  alias Phoenix.PubSub

  def mount(_params, _session, socket) do
    if connected?(socket) do
      PubSub.subscribe(Slip.PubSub, "slip")
    end

    assigns = [
      indicator_on: false,
      joystick_lh: 50,
      joystick_lv: 50,
      joystick_lp: :invalid,

      joystick_rh: 50,
      joystick_rv: 50
    ]

    {:ok, assign(socket, assigns)}
  end

  def handle_event("toggle-light", _params, socket) do
    on? = !socket.assigns.indicator_on
    Link.set_indicator on?
    {:noreply, assign(socket, indicator_on: on?)}
  end

  def handle_info({:joystick_lh, raw}, socket) do
    value = round(raw * -70 / 247 + 85)
    {:noreply, assign(socket, :joystick_lh, value)}
  end

  def handle_info({:joystick_lv, raw}, socket) do
    value = round(raw * -70 / 247 + 85)
    {:noreply, assign(socket, :joystick_lv, value)}
  end

  def handle_info({:joystick_rh, raw}, socket) do
    value = round(raw * -70 / 247 + 85)
    {:noreply, assign(socket, :joystick_rh, value)}
  end

  def handle_info({:joystick_rv, raw}, socket) do
    value = round(raw * -70 / 247 + 85)
    {:noreply, assign(socket, :joystick_rv, value)}
  end

  def handle_info(:unknown, socket) do
    {:noreply, socket}
  end
end
