/* This may look like C code, but it is really -*- C++ -*- */

/* Search algorithm.

   Copyright (C) 1989-1998, 2000, 2002 Free Software Foundation, Inc.
   Written by Douglas C. Schmidt <schmidt@ics.uci.edu>
   and Bruno Haible <bruno@clisp.org>.

   This file is part of GNU GPERF.

   GNU GPERF is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU GPERF is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.
   If not, write to the Free Software Foundation, Inc.,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#ifndef search_h
#define search_h 1

#include "keyword-list.h"
#include "bool-array.h"

class Search
{
public:
                        Search (KeywordExt_List *list);
                        ~Search ();
  void                  optimize ();
private:
  void                  prepare ();

  /* Merges two sorted lists together to form one sorted list.  */
  KeywordExt_List *     merge (KeywordExt_List *list1, KeywordExt_List *list2) const;
  /* Sorts a list using the recursive merge sort algorithm.  */
  KeywordExt_List *     merge_sort (KeywordExt_List *head) const;

  /* Computes the sum of occurrences of the _selchars of a keyword.  */
  int                   compute_occurrence (KeywordExt *ptr) const;

  /* Auxiliary functions used by Search::reorder().  */
  void                  clear_determined ();
  void                  set_determined (KeywordExt *keyword);
  bool                  already_determined (KeywordExt *keyword) const;
  /* Reorders the keyword list so as to minimize search times.  */
  void                  reorder ();

  /* Returns the length of keyword list.  */
  int                   keyword_list_length () const;

  /* Returns the maximum length of keywords.  */
  int                   max_key_length () const;

  /* Returns the number of key positions.  */
  int                   get_max_keysig_size () const;

  /* Initializes the asso_values[] related parameters and put a first guess
     into asso_values[].  */
  void                  init_asso_values ();

  /* Computes a keyword's hash value, relative to the current _asso_values[],
     and stores it in keyword->_hash_value.  */
  int                   compute_hash (KeywordExt *keyword) const;

  /* Sorts the given set in increasing frequency of _occurrences[].  */
  void                  sort_by_occurrence (unsigned char *set, int len) const;

  /* Tries various other values for _asso_values[c].  */
  bool                  try_asso_value (unsigned char c, KeywordExt *curr, int iterations);

  /* Attempts to change an _asso_value[], in order to resolve a hash value
     collision between the two given keywords.  */
  void                  change_some_asso_value (KeywordExt *prior, KeywordExt *curr);

  /* Finds good _asso_values[].  */
  void                  find_asso_values ();

  /* Sorts the keyword list by hash value.  */
  void                  sort ();

public:

  /* Linked list of keywords.  */
  KeywordExt_List *     _head;

  /* Total number of keywords, counting duplicates.  */
  int                   _total_keys;

  /* Total number of duplicates that have been moved to _duplicate_link lists
     (not counting their representatives which stay on the main list).  */
  int                   _total_duplicates;

  /* Maximum length of the longest keyword.  */
  int                   _max_key_len;

  /* Minimum length of the shortest keyword.  */
  int                   _min_key_len;

  /* Size of alphabet.  */
  int const             _alpha_size;

  /* Counts occurrences of each key set character.
     _occurrences[c] is the number of times that c occurs among the _selchars
     of a keyword.  */
  int * const           _occurrences;
  /* Value associated with each character. */
  int * const           _asso_values;

private:

  /* Length of _head list.  Number of keywords, not counting duplicates.  */
  int                   _list_len;

  /* Choice of sorting criterion during Search::merge_sort.  */
  /* True if sorting by occurrence.  */
  bool                  _occurrence_sort;
  /* True if sorting by hash value.  */
  bool                  _hash_sort;

  /* Vector used during Search::reorder().  */
  bool * const          _determined;

  /* Exclusive upper bound for every _asso_values[c].  A power of 2.  */
  int                   _asso_value_max;

  /* Maximal possible hash value.  */
  int                   _max_hash_value;

  /* Sparse bit vector for collision detection.  */
  Bool_Array *          _collision_detector;

  /* Minimal number of collisions found so far.  */
  int                   _fewest_collisions;

  /* Scratch set, used during Search::change_some_asso_value.  */
  unsigned char *       _union_set;

  /* Number of keyword being handled during Search::find_asso_values.  */
  int                   _num_done;
};

#endif
