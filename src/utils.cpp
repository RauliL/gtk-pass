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
#include "./utils.hpp"

#include <cstring>
#include <iostream>

#include <glob.h>
#include <wordexp.h>

static bool
expand_tilde(const std::string& input, std::string& output)
{
  ::wordexp_t result;
  int wordexp_return_value;

  wordexp_return_value = ::wordexp(input.c_str(), &result, WRDE_NOCMD);
  if (wordexp_return_value != 0)
  {
    ::wordfree(&result);
    std::cerr
      << "wordexp() failed with return value "
      << wordexp_return_value
      << std::endl;

    return false;
  }
  output = result.we_wordv[0];
  ::wordfree(&result);

  return true;
}

bool
glob(const std::string& pattern, std::vector<Glib::ustring>& results)
{
  ::glob_t glob_result;
  int glob_return_value;

  std::memset(static_cast<void*>(&glob_result), 0, sizeof(glob_result));

  glob_return_value = ::glob(
    pattern.c_str(),
    GLOB_TILDE,
    nullptr,
    &glob_result
  );
  if (glob_return_value != 0)
  {
    ::globfree(&glob_result);
    if (glob_return_value != GLOB_NOMATCH)
    {
      std::cerr
        << "glob() failed with return value "
        << glob_return_value
        << std::endl;
    }

    return false;
  }

  for (std::size_t i = 0; i < glob_result.gl_pathc; ++i)
  {
    results.push_back(glob_result.gl_pathv[i]);
  }

  ::globfree(&glob_result);

  return true;
}

bool
get_password_store_dir(std::string& output)
{
  std::string raw_path("~/.password-store");
  std::string expanded_path;

  if (const auto env_raw_path = std::getenv("PASSWORD_STORE_DIR"))
  {
    raw_path = env_raw_path;
  }
  if (expand_tilde(raw_path, expanded_path))
  {
    output = expanded_path;

    return true;
  }

  return false;
}
