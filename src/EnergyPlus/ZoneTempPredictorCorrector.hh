// EnergyPlus, Copyright (c) 1996-2022, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef ZoneTempPredictorCorrector_hh_INCLUDED
#define ZoneTempPredictorCorrector_hh_INCLUDED

// C++ Headers
#include <string>
#include <unordered_set>
#include <vector>

// ObjexxFCL Headers
#include <ObjexxFCL/Array1D.hh>
#include <ObjexxFCL/Array2D.hh>

// EnergyPlus Headers
#include <EnergyPlus/Data/BaseData.hh>
#include <EnergyPlus/DataGlobals.hh>
#include <EnergyPlus/DataHeatBalFanSys.hh>
#include <EnergyPlus/EnergyPlus.hh>

namespace EnergyPlus {

// Forward declarations
struct EnergyPlusData;

namespace ZoneTempPredictorCorrector {

    struct ZoneTempControl
    {
        std::string Name;          // Name of the zone
        std::string TempSchedName; // Name of the schedule which determines the zone temp setpoint
        int TempSchedIndex = 0;
        std::string HeatTempSetptSchedName;
        int HeatTempSchedIndex = 0;
        std::string CoolTempSetptSchedName;
        int CoolTempSchedIndex;
    };

    struct ZoneComfortFangerControl
    {
        std::string Name;                  // Name of the zone
        std::string PMVSchedName;          // Name of the schedule which determines the zone temp setpoint
        int PMVSchedIndex = 0;             // Index to PMV dual set point schedule
        std::string HeatPMVSetptSchedName; // Name of PMV heating set point schedule
        int HeatPMVSchedIndex = 0;         // Index to PMV heating set point schedule
        std::string CoolPMVSetptSchedName; // Name of PMV cooling set point schedule
        int CoolPMVSchedIndex;             // INdex to PMV cooling set point schedule
    };

    struct AdaptiveComfortDailySetPointSchedule
    {
        bool initialized = false;
        Array1D<Real64> ThermalComfortAdaptiveASH55_Upper_90;
        Array1D<Real64> ThermalComfortAdaptiveASH55_Upper_80;
        Array1D<Real64> ThermalComfortAdaptiveASH55_Central;
        Array1D<Real64> ThermalComfortAdaptiveCEN15251_Upper_I;
        Array1D<Real64> ThermalComfortAdaptiveCEN15251_Upper_II;
        Array1D<Real64> ThermalComfortAdaptiveCEN15251_Upper_III;
        Array1D<Real64> ThermalComfortAdaptiveCEN15251_Central;
    };

    struct SumHATOutput
    {
        // Output results from calSumHAT
        Real64 sumIntGain = 0.0;
        Real64 sumHA = 0.0;
        Real64 sumHATsurf = 0.0;
        Real64 sumHATref = 0.0;
    };

    struct ZoneSpaceHeatBalanceData
    {
        // This entire struct is re-initialized during the simulation, so no static informat may be stored here (e.g. zone or space characteristics)

        // Zone air drybulb conditions variables
        Real64 MAT = DataHeatBalFanSys::ZoneInitialTemp;      // MEAN AIR TEMPERATURE (C)
        Real64 ZTAV = DataHeatBalFanSys::ZoneInitialTemp;     // Air Temperature Averaged over the Zone Time step
        Real64 ZT = DataHeatBalFanSys::ZoneInitialTemp;       // Air Temperature Averaged over the System Time Increment
        Real64 ZTAVComf = DataHeatBalFanSys::ZoneInitialTemp; // Air Temperature Averaged over the Zone Time step used
        // in thermal comfort models (currently Fang model only)
        Real64 XMAT = DataHeatBalFanSys::ZoneInitialTemp; // Temporary zone/space temperature to test convergence
        Real64 XM2T = DataHeatBalFanSys::ZoneInitialTemp;
        Real64 XM3T = DataHeatBalFanSys::ZoneInitialTemp;
        Real64 XM4T = DataHeatBalFanSys::ZoneInitialTemp;
        Real64 DSXMAT = DataHeatBalFanSys::ZoneInitialTemp; // Down Stepped MAT history storage
        Real64 DSXM2T = DataHeatBalFanSys::ZoneInitialTemp; // Down Stepped MAT history storage
        Real64 DSXM3T = DataHeatBalFanSys::ZoneInitialTemp; // Down Stepped MAT history storage
        Real64 DSXM4T = DataHeatBalFanSys::ZoneInitialTemp; // Down Stepped MAT history storage
        Real64 XMPT = DataHeatBalFanSys::ZoneInitialTemp;   // Zone/space air temperature at previous time step
        // Exact and Euler solutions
        Real64 ZoneTMX = DataHeatBalFanSys::ZoneInitialTemp; // Temporary zone/space temperature to test convergence in Exact and Euler method
        Real64 ZoneTM2 = DataHeatBalFanSys::ZoneInitialTemp; // Temporary zone/space temperature at timestep t-2 in Exact and Euler method
        Real64 ZoneT1 = 0.0;                                 // Zone/space temperature at the previous time step used in Exact and Euler method

        // Zone Air moisture conditions variables
        Real64 ZoneAirHumRat = 0.01;        // Air Humidity Ratio
        Real64 ZoneAirHumRatAvg = 0.01;     // Air Humidity Ratio averaged over the zone time step
        Real64 ZoneAirHumRatTemp = 0.01;    // Temp air humidity ratio at time plus 1
        Real64 ZoneAirHumRatOld = 0.01;     // Last Time Steps air Humidity Ratio
        Real64 ZoneAirHumRatAvgComf = 0.01; // Air Humidity Ratio averaged over the zone time
                                            // step used in thermal comfort models (currently Fang model only)

        Real64 WZoneTimeMinus1 = 0.0;   // Humidity ratio history terms for 3rd order derivative
        Real64 WZoneTimeMinus2 = 0.0;   // Time Minus 2 Zone Time Steps Term
        Real64 WZoneTimeMinus3 = 0.0;   // Time Minus 3 Zone Time Steps Term
        Real64 WZoneTimeMinus4 = 0.0;   // Time Minus 4 Zone Time Steps Term
        Real64 DSWZoneTimeMinus1 = 0.0; // DownStepped Humidity ratio history terms for 3rd order derivative
        Real64 DSWZoneTimeMinus2 = 0.0; // DownStepped Time Minus 2 Zone Time Steps Term
        Real64 DSWZoneTimeMinus3 = 0.0; // DownStepped Time Minus 3 Zone Time Steps Term
        Real64 DSWZoneTimeMinus4 = 0.0; // DownStepped Time Minus 4 Zone Time Steps Term
        Real64 WZoneTimeMinusP = 0.0;   // Humidity ratio history terms at previous time step
        // Exact and Euler solutions
        Real64 ZoneWMX = 0.0; // Temporary humidity ratio to test convergence in Exact and Euler method
        Real64 ZoneWM2 = 0.0; // Temporary humidity ratio at timestep t-2 in Exact and Euler method
        Real64 ZoneW1 = 0.0;  // Zone/space humidity ratio at the previous time step used in Exact and Euler method

        Real64 ZTM1 = 0.0;                // zone air temperature at previous timestep
        Real64 ZTM2 = 0.0;                // zone air temperature at timestep T-2
        Real64 ZTM3 = 0.0;                // zone air temperature at previous T-3
        Real64 WZoneTimeMinus1Temp = 0.0; // Zone air humidity ratio at previous timestep
        Real64 WZoneTimeMinus2Temp = 0.0; // Zone air humidity ratio at timestep T-2
        Real64 WZoneTimeMinus3Temp = 0.0; // Zone air humidity ratio at timestep T-3

        Real64 SumIntGain = 0.0; // Sum of convective internal gains
        Real64 SumHA = 0.0;      // Sum of Hc*Area
        Real64 SumHATsurf = 0.0; // Sum of Hc*Area*Tsurf
        Real64 SumHATref = 0.0;  // Sum of Hc*Area*Tref= 0.0; for ceiling diffuser convection correlation
        Real64 SumMCp = 0.0;     // Sum of MassFlowRate*Cp
        Real64 SumMCpT = 0.0;    // Sum of MassFlowRate*Cp*T
        Real64 SumSysMCp = 0.0;  // Sum of air system MassFlowRate*Cp
        Real64 SumSysMCpT = 0.0; // Sum of air system MassFlowRate*Cp*T
        Real64 SumIntGainExceptPeople = 0.0;

        // Moisture variables to carry info from HB to the Zone Temp Predictor-Corrector for Fan System
        Real64 SumHmAW = 0.0;   // SUM OF ZONE AREA*Moist CONVECTION COEFF*INSIDE Humidity Ratio
        Real64 SumHmARa = 0.0;  // SUM OF ZONE AREA*Moist CONVECTION COEFF*Rho Air
        Real64 SumHmARaW = 0.0; // SUM OF ZONE AREA*Moist CONVECTION COEFF*Rho Air* Inside Humidity Ration
        Real64 SumHmARaZ = 0.0;

        Real64 TempDepCoef = 0.0; // Temperature dependent coefficient
        Real64 TempIndCoef = 0.0; // Temperature ndependent coefficient
        Real64 TempHistoryTerm = 0.0;

        Real64 MCPI = 0.0;                       // INFILTRATION MASS FLOW * AIR SPECIFIC HEAT
        Real64 MCPTI = 0.0;                      // INFILTRATION MASS FLOW * AIR CP * AIR TEMPERATURE
        Real64 MCPV = 0.0;                       // VENTILATION MASS FLOW * AIR SPECIFIC HEAT
        Real64 MCPTV = 0.0;                      // VENTILATION MASS FLOW * AIR CP * AIR TEMPERATURE
        Real64 MCPM = 0.0;                       // Mixing MASS FLOW * AIR SPECIFIC HEAT
        Real64 MCPTM = 0.0;                      // Mixing MASS FLOW * AIR CP * AIR TEMPERATURE
        Real64 MCPE = 0.0;                       // EARTHTUBE MASS FLOW * AIR SPECIFIC HEAT
        Real64 EAMFL = 0.0;                      // OUTDOOR AIR MASS FLOW for EarthTube
        Real64 EAMFLxHumRat = 0.0;               // OUTDOOR AIR MASS FLOW * Humidity Ratio for EarthTube (water vapor mass flow)
        Real64 MCPTE = 0.0;                      // EARTHTUBE MASS FLOW * AIR CP * AIR TEMPERATURE
        Real64 MCPC = 0.0;                       // COOLTOWER MASS FLOW * AIR SPECIFIC HEAT
        Real64 CTMFL = 0.0;                      // OUTDOOR AIR MASS FLOW for cooltower
        Real64 MCPTC = 0.0;                      // COOLTOWER MASS FLOW * AIR CP * AIR TEMPERATURE
        Real64 ThermChimAMFL = 0.0;              // OUTDOOR AIR MASS FLOW for THERMALCHIMNEY
        Real64 MCPTThermChim = 0.0;              // THERMALCHIMNEY MASS FLOW * AIR SPECIFIC HEAT
        Real64 MCPThermChim = 0.0;               // THERMALCHIMNEY MASS FLOW * AIR CP * AIR TEMPERATURE
        Real64 ZoneLatentGain = 0.0;             // Latent Energy from each Zone (People, equipment)
        Real64 ZoneLatentGainExceptPeople = 0.0; // Added for hybrid model -- Latent Energy from each Zone (equipment)
        Real64 OAMFL = 0.0;                      // OUTDOOR AIR MASS FLOW (kg/s) for infiltration
        Real64 VAMFL = 0.0;                      // OUTDOOR AIR MASS FLOW (kg/s) for ventilation
        Real64 NonAirSystemResponse = 0.0;       // Convective heat addition rate from non forced air
        // equipment such as baseboards plus heat from lights to
        Real64 SysDepZoneLoads = 0.0; // Convective heat addition or subtraction rate from sources that
        // depend on what is happening with the HVAC system. Such as:
        // heat gain from lights to return air when return flow = 0= 0.0; heat gain
        // from air flow windows to return air when return air flow = 0= 0.0;
        // and heat removed by return air from refrigeration cases when
        // return air flow = 0.
        Real64 SysDepZoneLoadsLagged = 0.0; // SysDepZoneLoads saved to be added to zone heat balance next
        // HVAC time step
        Real64 MDotCPOA = 0.0; // Airbalance MASS FLOW * AIR SPECIFIC HEAT used at Air Balance Method = Quadrature in the ZoneAirBalance:OutdoorAir
        Real64 MDotOA = 0.0;   // Airbalance MASS FLOW rate used at Air Balance Method = Quadrature in the ZoneAirBalance:OutdoorAir
        Real64 MixingMAT = DataHeatBalFanSys::ZoneInitialTemp; // Air temperature for mixing
        Real64 MixingHumRat = 0.01;                            // Air humidity ratio for mixing
        Real64 MixingMassFlowZone = 0.0;                       // Mixing MASS FLOW (kg/s)
        Real64 MixingMassFlowXHumRat = 0.0;                    // Mixing MASS FLOW * Humidity Ratio

        Real64 ZoneSetPointLast = 0.0;
        Real64 TempIndZnLd = 0.0;
        Real64 TempDepZnLd = 0.0;
        Real64 ZoneAirRelHum = 0.0; // Zone relative humidity in percent
        Real64 AirPowerCap = 0.0;   // "air power capacity"  Vol*VolMult*rho*Cp/timestep [W/degK]

        void beginEnvironmentInit(EnergyPlusData &state);

        void calcPredictedSystemLoad(EnergyPlusData &state, Real64 const RAFNFrac, int const zoneNum, int const spaceNum = 0);

        void calcZoneOrSpaceSums(EnergyPlusData &state,
                                 bool const CorrectorFlag, // Corrector call flag
                                 int const zoneNum,
                                 int const spaceNum = 0);

        virtual SumHATOutput
        calcSumHAT([[maybe_unused]] EnergyPlusData &state, [[maybe_unused]] int const zoneNum, [[maybe_unused]] int const spaceNum) = 0;

        void updateTemperatures(EnergyPlusData &state,
                                bool const ShortenTimeStepSys,
                                bool const UseZoneTimeStepHistory,
                                Real64 const PriorTimeStep,
                                int const zoneNum,
                                int const spaceNum = 0);

        Real64 correctAirTemp(EnergyPlusData &state,
                              bool useZoneTimeStepHistory, // if true then use zone timestep history, if false use system time step history
                              int const zoneNum,
                              int const spaceNum = 0);

        void calcPredictedHumidityRatio(EnergyPlusData &state, Real64 const RAFNFrac, int const zoneNum, int const spaceNum = 0);
    };

    struct ZoneHeatBalanceData : ZoneSpaceHeatBalanceData
    {
        SumHATOutput calcSumHAT(EnergyPlusData &state, int const zoneNum, [[maybe_unused]] int const spaceNum) override;
    };

    struct SpaceHeatBalanceData : ZoneSpaceHeatBalanceData
    {
        SumHATOutput calcSumHAT(EnergyPlusData &state, int const zoneNum, int const spaceNum) override;
    };

    // Functions

    void ManageZoneAirUpdates(EnergyPlusData &state,
                              DataHeatBalFanSys::PredictorCorrectorCtrl UpdateType, // Can be iGetZoneSetPoints, iPredictStep, iCorrectStep
                              Real64 &ZoneTempChange,                               // Temp change in zone air btw previous and current timestep
                              bool ShortenTimeStepSys,
                              bool UseZoneTimeStepHistory, // if true then use zone timestep history, if false use system time step
                              Real64 PriorTimeStep         // the old value for timestep length is passed for possible use in interpolating
    );

    void GetZoneAirSetPoints(EnergyPlusData &state);

    void InitZoneAirSetPoints(EnergyPlusData &state);

    void PredictSystemLoads(EnergyPlusData &state,
                            bool ShortenTimeStepSys,
                            bool UseZoneTimeStepHistory, // if true then use zone timestep history, if false use system time step
                            Real64 PriorTimeStep         // the old value for timestep length is passed for possible use in interpolating
    );

    void CalcZoneAirTempSetPoints(EnergyPlusData &state);

    void CalculateMonthlyRunningAverageDryBulb(EnergyPlusData &state, Array1D<Real64> &runningAverageASH, Array1D<Real64> &runningAverageCEN);

    void
    CalculateAdaptiveComfortSetPointSchl(EnergyPlusData &state, Array1D<Real64> const &runningAverageASH, Array1D<Real64> const &runningAverageCEN);

    Real64 correctZoneAirTemps(EnergyPlusData &state,
                               bool useZoneTimeStepHistory // if true then use zone timestep history, if false use system time step history
    );

    void PushZoneTimestepHistories(EnergyPlusData &state);

    void PushSystemTimestepHistories(EnergyPlusData &state);

    void RevertZoneTimestepHistories(EnergyPlusData &state);

    void CorrectZoneHumRat(EnergyPlusData &state, int ZoneNum);

    void DownInterpolate4HistoryValues(Real64 OldTimeStep,
                                       Real64 NewTimeStep,
                                       Real64 &oldVal0,
                                       Real64 &oldVal1,
                                       Real64 &oldVal2,
                                       Real64 &newVal0,
                                       Real64 &newVal1,
                                       Real64 &newVal2,
                                       Real64 &newVal3, // unused 1208
                                       Real64 &newVal4  // unused 1208
    );

    void InverseModelTemperature(EnergyPlusData &state,
                                 int ZoneNum,                         // Zone number
                                 Real64 const SumIntGain,             // Zone sum of convective internal gains
                                 Real64 const SumIntGainExceptPeople, // Zone sum of convective internal gains except for people
                                 Real64 const SumHA,                  // Zone sum of Hc*Area
                                 Real64 const SumHATsurf,             // Zone sum of Hc*Area*Tsurf
                                 Real64 const SumHATref,              // Zone sum of Hc*Area*Tref, for ceiling diffuser convection correlation
                                 Real64 const SumMCp,                 // Zone sum of MassFlowRate*Cp
                                 Real64 const SumMCpT,                // Zone sum of MassFlowRate*Cp*T
                                 Real64 const SumSysMCp,              // Zone sum of air system MassFlowRate*Cp
                                 Real64 const SumSysMCpT,             // Zone sum of air system MassFlowRate*Cp*T
                                 Real64 const AirCap                  // Formerly CoefAirrat, coef in zone temp eqn with dim of "air power capacity"rd
    );

    void InverseModelHumidity(EnergyPlusData &state,
                              int ZoneNum,                         // Zone number
                              Real64 const LatentGain,             // Zone sum of latent gain
                              Real64 const LatentGainExceptPeople, // Zone sum of latent gain except for people
                              Real64 const ZoneMassFlowRate,       // Zone air mass flow rate
                              Real64 const MoistureMassFlowRate,   // Zone moisture mass flow rate
                              Real64 const H2OHtOfVap,             // Heat of vaporization of air
                              Real64 const RhoAir                  // Air density
    );

    void CalcZoneComponentLoadSums(EnergyPlusData &state,
                                   int ZoneNum,             // Zone number
                                   Real64 TempDepCoef,      // Dependent coefficient
                                   Real64 TempIndCoef,      // Independent coefficient
                                   Real64 &SumIntGains,     // Zone sum of convective internal gains
                                   Real64 &SumHADTsurfs,    // Zone sum of Hc*Area*(Tsurf - Tz)
                                   Real64 &SumMCpDTzones,   // zone sum of MassFlowRate*cp*(TremotZone - Tz) transfer air from other zone, Mixing
                                   Real64 &SumMCpDtInfil,   // Zone sum of MassFlowRate*Cp*(Tout - Tz) transfer from outside, ventil, earth tube
                                   Real64 &SumMCpDTsystem,  // Zone sum of air system MassFlowRate*Cp*(Tsup - Tz)
                                   Real64 &SumNonAirSystem, // Zone sum of non air system convective heat gains
                                   Real64 &CzdTdt,          // Zone air energy storage term.
                                   Real64 &imBalance,       // put all terms in eq. 5 on RHS , should be zero
                                   Real64 &SumEnthalpyM,    // Zone sum of phase change material melting enthlpy
                                   Real64 &SumEnthalpyH     // Zone sum of phase change material freezing enthalpy
    );

    bool VerifyThermostatInZone(EnergyPlusData &state, std::string const &ZoneName); // Zone to verify

    bool VerifyControlledZoneForThermostat(EnergyPlusData &state, std::string const &ZoneName); // Zone to verify

    void DetectOscillatingZoneTemp(EnergyPlusData &state);

    void AdjustAirSetPointsforOpTempCntrl(EnergyPlusData &state, int TempControlledZoneID, int ActualZoneNum, Real64 &ZoneAirSetPoint);

    void AdjustOperativeSetPointsforAdapComfort(EnergyPlusData &state, int TempControlledZoneID, Real64 &ZoneAirSetPoint);

    void CalcZoneAirComfortSetPoints(EnergyPlusData &state);

    void GetComfortSetPoints(EnergyPlusData &state,
                             int PeopleNum,
                             int ComfortControlNum,
                             Real64 PMVSet,
                             Real64 &Tset // drybulb setpoint temperature for a given PMV value
    );

    Real64 PMVResidual(EnergyPlusData &state,
                       Real64 Tset,
                       Array1D<Real64> const &Par // par(1) = PMV set point
    );

    void AdjustCoolingSetPointforTempAndHumidityControl(EnergyPlusData &state,
                                                        int TempControlledZoneID,
                                                        int ActualZoneNum // controlled zone actual zone number
    );

    void OverrideAirSetPointsforEMSCntrl(EnergyPlusData &state);

    void FillPredefinedTableOnThermostatSetpoints(EnergyPlusData &state);

    std::tuple<Real64, int, std::string>
    temperatureAndCountInSch(EnergyPlusData &state, int scheduleIndex, bool isSummer, int dayOfWeek, int hourOfDay);

} // namespace ZoneTempPredictorCorrector

struct ZoneTempPredictorCorrectorData : BaseGlobalStruct
{
    int NumSingleTempHeatingControls = 0;
    int NumSingleTempCoolingControls = 0;
    int NumSingleTempHeatCoolControls = 0;
    int NumDualTempHeatCoolControls = 0;

    // Number of Thermal comfort control types
    int NumSingleFangerHeatingControls = 0;
    int NumSingleFangerCoolingControls = 0;
    int NumSingleFangerHeatCoolControls = 0;
    int NumDualFangerHeatCoolControls = 0;

    // Number of zone with staged controlled objects
    int NumStageCtrZone = 0;
    // Number of zone with onoff thermostat
    int NumOnOffCtrZone = 0;

    // Zone temperature history - used only for oscillation test
    Array2D<Real64> ZoneTempHist;
    Array1D<Real64> ZoneTempOscillate;
    Array1D<Real64> ZoneTempOscillateDuringOccupancy;
    Array1D<Real64> ZoneTempOscillateInDeadband;
    Real64 AnyZoneTempOscillate = 0.0;
    Real64 AnyZoneTempOscillateDuringOccupancy = 0.0;
    Real64 AnyZoneTempOscillateInDeadband = 0.0;
    Real64 AnnualAnyZoneTempOscillate = 0.0;
    Real64 AnnualAnyZoneTempOscillateDuringOccupancy = 0.0;
    Real64 AnnualAnyZoneTempOscillateInDeadband = 0.0;
    bool OscillationVariablesNeeded = false;

    bool InitZoneAirSetPointsOneTimeFlag = true;
    bool SetupOscillationOutputFlag = true;

    // Object Data
    std::unordered_set<std::string> HumidityControlZoneUniqueNames;
    EPVector<ZoneTempPredictorCorrector::ZoneTempControl> SetPointSingleHeating;
    EPVector<ZoneTempPredictorCorrector::ZoneTempControl> SetPointSingleCooling;
    EPVector<ZoneTempPredictorCorrector::ZoneTempControl> SetPointSingleHeatCool;
    EPVector<ZoneTempPredictorCorrector::ZoneTempControl> SetPointDualHeatCool;
    EPVector<ZoneTempPredictorCorrector::ZoneComfortFangerControl> SetPointSingleHeatingFanger;
    EPVector<ZoneTempPredictorCorrector::ZoneComfortFangerControl> SetPointSingleCoolingFanger;
    EPVector<ZoneTempPredictorCorrector::ZoneComfortFangerControl> SetPointSingleHeatCoolFanger;
    EPVector<ZoneTempPredictorCorrector::ZoneComfortFangerControl> SetPointDualHeatCoolFanger;
    ZoneTempPredictorCorrector::AdaptiveComfortDailySetPointSchedule AdapComfortDailySetPointSchedule;

    std::array<Real64, 7> AdapComfortSetPointSummerDesDay = {-1};

    bool CalcZoneAirComfortSetPointsFirstTimeFlag = true; // Flag set to make sure you get input once
    bool MyEnvrnFlag = true;
    bool MyDayFlag = true;
    bool ErrorsFound = false;
    bool ControlledZonesChecked = false;

    int IterLimitExceededNum1 = 0;
    int IterLimitErrIndex1 = 0;
    int IterLimitExceededNum2 = 0;
    int IterLimitErrIndex2 = 0;

    EPVector<ZoneTempPredictorCorrector::ZoneHeatBalanceData> zoneHeatBalance;
    EPVector<ZoneTempPredictorCorrector::SpaceHeatBalanceData> spaceHeatBalance;

    void clear_state() override
    {
        *this = ZoneTempPredictorCorrectorData();
    }
};

} // namespace EnergyPlus

#endif
