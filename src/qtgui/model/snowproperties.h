#ifndef SNOWPROPERTIES_H
#define SNOWPROPERTIES_H

#include <mitsuba/mitsuba.h>

MTS_NAMESPACE_BEGIN

struct SnowProperties {
    MTS_DECLARE_CLASS()

    /* Some preset configurations */
    enum EPreset {
        EFreshNewSnow = 0,
        EDryOlderSnow = 1,
        EWetOldSnow = 2,
        ECustom = 3
    };

    /* Different calculation modes */
    enum ECalculationMode {
        EPhenomenological = 0,
        EAsymptotic = 1,
        ESnowPack = 2,
        ELargeParticle =3
    };

    /* grain diameter in m */
    Float grainsize;
    /* density in kg / m^3 */
    Float density;
    /* IOR */
    Float ior;
    /* asymmetry factor g (mean cosine of phase function */
    Float g;
    /* absorption coefficient */
    Spectrum sigmaA;
    /* scattering coefficient */
    Spectrum sigmaS;
    /* extinction coefficient, sum of sigmaA and sigmaS */
    Spectrum sigmaT;
    /* single scattering albedo */
    Spectrum singleScatteringAlbedo;
    /* absorbtion coefficient of ice */
    static Spectrum iceSigmaA;
    /* density of ice */
    static Float iceDensity;
    /* last breset associated with an instance */
    EPreset lastPreset;
    /* calculation mode of coefficients */
    ECalculationMode calcMode;
    /* indicates if the subsurface scattering should be calculated automaticly */
    bool ssOverride;
    /* a potential override value for the single scattering albedo */
    Float ssAlbedoOverride;

    SnowProperties();

    SnowProperties(EPreset preset);

    SnowProperties(Float _grainsize, Float _density, Float _ior, Float _g);

    void loadPreset(EPreset preset);

    void loadFreshNewSnowPreset();

    void loadDryOlderSnowPreset();

    void loadWetOldSnowPreset();

    void configure();

    std::string toString();
};

MTS_NAMESPACE_END

#endif // SNOWPROPERTIES_H
