/*****************************************************************************
 * resource.h                         resource header for AppDep application *
 * Author               Julien Darthenay julien<dot>darthenay<at>free<dot>fr *
 * Developing tools              MinGW-w64, Notepad++, MS Windows Seven Home *
 * Copyright 2010-2016 Julien Darthenay                                      *
 *****************************************************************************/

/*****************************************************************************
 * This file is part of AppDep                                               *
 *                                                                           *
 * AppDep is free software: you can redistribute it and/or modify it         *
 * under the terms of the GNU General Public License as published by         *
 * the Free Software Foundation, either version 3 of the License, or (at     *
 * your option) any later version.                                           *
 *                                                                           *
 * AppDep is distributed in the hope that it will be useful, but WITHOUT     *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public              *
 * License for more details.                                                 *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with AppDep. If not, see <http://www.gnu.org/licenses/>.            *
 *****************************************************************************/

/**
 * \file resource.h
 * \brief Resource header for AppDep application.
 * \author <a href="mailto:julien.darthenay@free.fr">Julien Darthenay</a>
 * \since 1.0
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#ifndef VS_VERSION_INFO

/**
 * \brief Version number resource.
 * \since 1.0
 */
#define VS_VERSION_INFO 1
#endif

#ifndef CREATEPROCESS_MANIFEST_RESOURCE_ID

/**
 * \brief XP manifest resource.
 * \since 1.0
 */
#define CREATEPROCESS_MANIFEST_RESOURCE_ID 1
#endif

#ifndef RT_MANIFEST

/**
 * \brief XP manifest resource.
 * \since 1.0
 */
#define RT_MANIFEST 24
#endif

#endif /* RESOURCE_H */
