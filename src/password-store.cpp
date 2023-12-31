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
#include <iostream>

#include <unistd.h>

#include "./password-store.hpp"
#include "./utils.hpp"

PasswordStore::PasswordStore()
{
  reload();
}

void
PasswordStore::reload()
{
  std::string prefix;

  if (get_password_store_dir(prefix))
  {
    m_container.clear();
    if (glob(prefix + "/**/*.gpg", m_container))
    {
      // We got some entries, but they have are full filenames. Lets tweak them
      // a little bit.
      for (auto& entry : m_container)
      {
        // Remove the password store directory from the entry.
        entry = entry.substr(prefix.length() + 1);
        // Remove the ".gpg" extension from the entry.
        entry = entry.substr(0, entry.length() - 4);
      }
    }
  } else {
    std::cerr << "Unable to determine location of passwords." << std::endl;
  }
}

void
PasswordStore::select(const_reference entry)
{
  if (has(entry))
  {
    ::execlp("pass", "pass", "show", "-c", entry.c_str(), nullptr);
  }
}
