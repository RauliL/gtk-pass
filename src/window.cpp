/*
 * Copyright (c) 2023-2026, Rauli Laine
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "./window.hpp"

Window::Window()
  : m_store(std::make_shared<PasswordStore>())
  , m_box(Gtk::Orientation::VERTICAL)
  , m_search_entry_completion(EntryCompletion::create(m_store))
  , m_entry_list(m_store)
{
  auto controller = Gtk::EventControllerKey::create();

  set_title("Select password");
  set_default_size(450, 250);

  m_box.append(m_search_bar);
  m_box.append(m_entry_list);

  m_search_bar.set_margin(8);
  m_search_bar.append(m_search_entry);

  m_search_entry.set_hexpand(true);
  m_search_entry.set_icon_from_icon_name("edit-find-symbolic");
  m_search_entry.set_completion(m_search_entry_completion);

  set_child(m_box);

  m_search_entry.signal_changed().connect(sigc::mem_fun(
    *this,
    &Window::on_search_text_changed
  ));
  m_search_entry.signal_activate().connect(sigc::mem_fun(
    *this,
    &Window::on_search_activated
  ));

  controller->signal_key_pressed().connect(
    sigc::mem_fun(*this, &Window::on_window_key_pressed),
    false
  );
  add_controller(controller);
}

bool
Window::on_window_key_pressed(
  guint keyval,
  guint keycode,
  Gdk::ModifierType state
)
{
  if ((state & Gdk::ModifierType::CONTROL_MASK) == Gdk::ModifierType::CONTROL_MASK)
  {
    // Terminate the application when user presses ^Q anywhere inside the
    // window.
    if (keyval == GDK_KEY_q)
    {
      std::exit(EXIT_SUCCESS);

      return true;
    }

    // Reload password store when user presses ^R anywhere inside the window.
    if (keyval == GDK_KEY_r)
    {
      m_store->reload();
      m_search_entry_completion->reload(m_store);
      m_entry_list.reload();

      return true;
    }
  }

  // Also terminate the application if user presses escape anywhere inside the
  // window.
  if (keyval == GDK_KEY_Escape)
  {
    std::exit(EXIT_SUCCESS);

    return true;
  }

  return false;
}

void
Window::on_search_text_changed()
{
  m_entry_list.set_filter(m_search_entry.get_text());
}

void
Window::on_search_activated()
{
  const auto& text = m_search_entry.get_text();

  if (text.length() > 0)
  {
    m_store->select(text);
  }
}
