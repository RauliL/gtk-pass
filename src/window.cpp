/*
 * Copyright (c) 2023, Rauli Laine
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

std::vector<std::string> read_pass_entries();

Window::Window()
  : m_box(Gtk::ORIENTATION_VERTICAL)
{
  set_title("Select password");
  set_default_size(450, 500);
  set_position(Gtk::WindowPosition::WIN_POS_CENTER);

  m_box.pack_start(m_search_bar, Gtk::PACK_SHRINK);
  m_box.pack_start(m_entry_list, Gtk::PACK_EXPAND_WIDGET);

  m_search_bar.add(m_search_entry);
  m_search_bar.connect_entry(m_search_entry);
  m_search_bar.set_show_close_button(false);
  m_search_bar.set_search_mode(true);

  add(m_box);
  show_all();

  m_search_entry.signal_search_changed().connect(sigc::mem_fun(
    this,
    &Window::on_search_text_changed
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
