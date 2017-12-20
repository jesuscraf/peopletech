// PAM module for two-factor authentication.
//
// Copyright 2010 Google Inc.
// Author: Markus Gutschke
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "compute.h"

#include "hmac.h"
#include "sha1.h"
#include <memory.h>


/* Given an input value, this function computes the hash code that forms the
 * expected authentication token.
 */

int compute_code(const uint8_t *secret, int secretLen, unsigned long value) {
  int i = 0;
  int offset = 0;
  unsigned int truncatedHash = 0;
  uint8_t val[8];
  uint8_t hash[SHA1_DIGEST_LENGTH];

  for (i = 8; i--; value >>= 8) {
    val[i] = (uint8_t)value;
  }
  hmac_sha1(secret, secretLen, val, 8, hash, SHA1_DIGEST_LENGTH);
  memset(val, 0, sizeof(val));
  offset = hash[SHA1_DIGEST_LENGTH - 1] & 0xF;
  truncatedHash = 0;
  for (i = 0; i < 4; ++i) {
    truncatedHash <<= 8;
    truncatedHash  |= hash[offset + i];
  }
  memset(hash, 0, sizeof(hash));
  truncatedHash &= 0x7FFFFFFF;
  truncatedHash %= 1000000;
  return truncatedHash;
}

