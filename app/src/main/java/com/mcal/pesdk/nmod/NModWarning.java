/*
 * Copyright (C) 2018-2019 Тимашков Иван
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
package com.mcal.pesdk.nmod;

//##################################################################

/**
 * @author Тимашков Иван
 * @author https://github.com/TimScriptov
 */
public class NModWarning {
//##################################################################
    private int mType;
    private Throwable mCause;

    public NModWarning(int type, Throwable cause) {
        mType = type;
        mCause = cause;
    }

    public int getType() {
        return mType;
    }

    public Throwable getCause() {
        return mCause;
    }
}
