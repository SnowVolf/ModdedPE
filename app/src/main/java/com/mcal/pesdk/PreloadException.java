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
package com.mcal.pesdk;

public class PreloadException extends Exception {
    public static final int TYPE_LOAD_LIBS_FAILED = 1;
    public static final int TYPE_IO_EXCEPTION = 2;

    private int mType;

    public PreloadException(int type, Throwable cause) {
        super(cause);
        mType = type;
    }

    public int getType() {
        return mType;
    }
}
