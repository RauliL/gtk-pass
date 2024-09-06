/*
 * Copyright (c) 2023-2024, Rauli Laine
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
  , m_box(Gtk::ORIENTATION_VERTICAL)
  , m_search_entry_completion(EntryCompletion::create(m_store))
  , m_entry_list(m_store)
{
  set_title("Select password");
  set_default_size(450, 250);
  set_position(Gtk::WindowPosition::WIN_POS_CENTER);

  m_box.pack_start(m_search_bar, Gtk::PACK_SHRINK);
  m_box.pack_start(m_entry_list, Gtk::PACK_EXPAND_WIDGET);

  m_search_bar.set_margin_top(8);
  m_search_bar.set_margin_bottom(8);
  m_search_bar.set_margin_left(8);
  m_search_bar.set_margin_right(8);
  m_search_bar.pack_start(m_search_entry, Gtk::PACK_EXPAND_WIDGET);

  m_search_entry.set_icon_from_icon_name("edit-find-symbolic");
  m_search_entry.set_completion(m_search_entry_completion);

  add(m_box);
  show_all();

  m_search_entry.signal_changed().connect(sigc::mem_fun(
    this,
    &Window::on_search_text_changed
  ));
  m_search_entry.signal_activate().connect(sigc::mem_fun(
    this,
    &Window::on_search_activated
  ));
}

bool
Window::on_key_press_event(GdkEventKey* event)
{
  if ((event->state & GDK_CONTROL_MASK))
  {
    // Terminate the application when user presses ^Q anywhere inside the
    // window.
    if (event->keyval == GDK_KEY_q)
    {
      std::exit(EXIT_SUCCESS);

      return false;
    }

    // Reload password store when user presses ^R anywhere inside the window.
    if (event->keyval == GDK_KEY_r)
    {
      m_store->reload();
      m_search_entry_completion->reload(m_store);
      m_entry_list.reload();

      return false;
    }
  }

  // Also terminate the application if user presses escape anywhere inside the
  // window.
  if (event->keyval == GDK_KEY_Escape)
  {
    std::exit(EXIT_SUCCESS);

    return false;
  }

  return Gtk::Window::on_key_press_event(event);
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
