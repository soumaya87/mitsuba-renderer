/*
    This file is part of Mitsuba, a physically based rendering system.

    Copyright (c) 2007-2011 by Wenzel Jakob and others.

    Mitsuba is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License Version 3
    as published by the Free Software Foundation.

    Mitsuba is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#if !defined(__CONST_TEXTURE_H)
#define __CONST_TEXTURE_H

#include <mitsuba/core/properties.h>
#include <mitsuba/render/texture.h>

MTS_NAMESPACE_BEGIN

class MTS_EXPORT_RENDER ConstantSpectrumTexture : public Texture {
public:
	inline ConstantSpectrumTexture(const Spectrum &value)
		: Texture(Properties()), m_value(value) {
	}

	ConstantSpectrumTexture(Stream *stream, InstanceManager *manager);

	inline Spectrum getValue(const Intersection &its) const {
		return m_value;
	}

	inline Spectrum getAverage() const {
		return m_value;
	}
	
	inline Spectrum getMaximum() const {
		return m_value;
	}

	inline std::string toString() const {
		std::ostringstream oss;
		oss << "ConstantSpectrumTexture[value=" << m_value.toString() << "]";
		return oss.str();
	}

	inline bool usesRayDifferentials() const {
		return false;
	}

	Shader *createShader(Renderer *renderer) const;

	void serialize(Stream *stream, InstanceManager *manager) const;

	MTS_DECLARE_CLASS()
protected:
	Spectrum m_value;
};

class MTS_EXPORT_RENDER ConstantFloatTexture : public Texture {
public:
	inline ConstantFloatTexture(const Float &value)
		: Texture(Properties()), m_value(value) {
	}

	ConstantFloatTexture(Stream *stream, InstanceManager *manager);


	inline Spectrum getValue(const Intersection &its) const {
		return Spectrum(m_value);
	}

	inline Spectrum getAverage() const {
		return Spectrum(m_value);
	}
	
	inline Spectrum getMaximum() const {
		return Spectrum(m_value);
	}

	inline std::string toString() const {
		std::ostringstream oss;
		oss << "ConstantFloatTexture[value=" << m_value << "]";
		return oss.str();
	}

	inline bool usesRayDifferentials() const {
		return false;
	}

	Shader *createShader(Renderer *renderer) const;

	void serialize(Stream *stream, InstanceManager *manager) const;

	MTS_DECLARE_CLASS()
protected:
	Float m_value;
};


MTS_NAMESPACE_END

#endif /* __CONST_TEXTURE_H */
