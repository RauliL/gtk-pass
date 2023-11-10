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
#include "./config.hpp"
#include "./window.hpp"

#if ENABLE_SEARCH_BAR_HACK
static void
hack_search_bar(Gtk::SearchBar& search_bar)
{
  const auto& main_box = static_cast<Gtk::Container*>(
    static_cast<Gtk::Container*>(
      search_bar.get_children()[0]
    )->get_children()[0]
  );
  const auto& children = main_box->get_children();
  const auto& box1 = children[0];
  const auto& box2 = children[1];
  const auto& box3 = children[2];

  box2->set_hexpand(true);
  box2->set_halign(Gtk::Align::ALIGN_FILL);
  main_box->remove(*box1);
  box3->set_hexpand(false);
}
#endif /* ENABLE_SEARCH_BAR_HACK */

Window::Window()
  : m_pass_entries(PassEntries::create())
  , m_box(Gtk::ORIENTATION_VERTICAL)
  , m_search_entry_completion(EntryCompletion::create(m_pass_entries))
  , m_entry_list(m_pass_entries)
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

#if ENABLE_SEARCH_BAR_HACK
  hack_search_bar(m_search_bar);
#endif

  m_search_entry.set_completion(m_search_entry_completion);

  add(m_box);
  show_all();

  m_search_entry.signal_search_changed().connect(sigc::mem_fun(
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

  if (text.length() > 0 && m_pass_entries->has(text))
  {
    m_pass_entries->select(text);
  }
}
