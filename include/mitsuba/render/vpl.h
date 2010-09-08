/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2010 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(__VPL_H)
#define __VPL_H

#include <mitsuba/render/scene.h>

MTS_NAMESPACE_BEGIN

enum EVPLType {
	ELuminaireVPL = 0,
	ESurfaceVPL	
};

/**
 * Support routines for rendering algorithms based on VPLs (virtual
 * point lights)
 */
struct VPL {
	inline VPL(EVPLType type, const Spectrum &P) 
		: type(type), P(P) {
	}
	EVPLType type;
	Spectrum P;
	Intersection its;
	const Luminaire *luminaire;

	std::string toString() const;
};

/**
 * Generate a series of point light sources by sampling from the Halton
 * sequence (as is done in Instant Radiosity). The parameter <code>offset</code> 
 * allows setting the initial QMC sample index (should be set to 0 if no offset is
 * desired), and the last index is returned after the function finishes. This can 
 * be used to generate an arbitrary number of VPLs incrementally. Note that the 
 * parameter <code>count</code> is only a suggestion. Generally, the implementation 
 * will produce a few more VPLs. After VPL generation is done, their power must be scaled
 * by the inverse of the returned index.
 */
extern MTS_EXPORT_RENDER size_t generateVPLs(const Scene *scene, size_t offset, 
	size_t count, int maxDepth, std::deque<VPL> &vpls);

MTS_NAMESPACE_END

#endif /* __VPL_H */
