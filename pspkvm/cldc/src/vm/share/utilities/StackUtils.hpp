/*
 *   
 *
 * Copyright  1990-2007 Sun Microsystems, Inc. All Rights Reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 only, as published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License version 2 for more details (a copy is
 * included at /legal/license.txt).
 * 
 * You should have received a copy of the GNU General Public License
 * version 2 along with this work; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 * 
 * Please contact Sun Microsystems, Inc., 4150 Network Circle, Santa
 * Clara, CA 95054 or visit www.sun.com if you need additional
 * information or have any questions.
 */

class GlobalSaver : public StackObj {
protected:
  int  _value;
  int* _ptr;
  bool _bvalue;
  bool* _bptr;

public:
  GlobalSaver(int* ptr) : _value(*ptr), _ptr(ptr), _bvalue(false), _bptr(NULL) {}
  GlobalSaver(bool* ptr) : _value(0), _ptr(NULL), _bvalue(*ptr), _bptr(ptr) {}
  ~GlobalSaver() { if (_ptr) *_ptr = _value; if (_bptr) *_bptr = _bvalue; }
};

class TemporaryModifyGlobal : public GlobalSaver {
public:
  TemporaryModifyGlobal(int* ptr, int new_value) : GlobalSaver(ptr) {
    *_ptr = new_value;
  }
};

