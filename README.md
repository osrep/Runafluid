RUNAFLUID
Runaway Fluid

Purpose

This document specifies software requirements of the Runaway Fluid (runafluid) software module. The module is developed and deployed in the European Transport Solver (ETS) framework maintained by the Code Development for Integrated Modelling Project (ITM) of the EUROfusion consortium (https://portal.eufus.eu/itm/portal/).

Intended Audience and Reading Suggestions

This Software Requirements Specification (SRS) document is intended for ITM/ETS/HCD workflow developers, and developers of runafluid module. The document describes the module runafluid, laying out functional and non-functional requirements. Purpose, overview of the module, interfaces, and CPO objects used, constraints, assumptions and dependencies, functional requirements are contained in this documentation.

Product Scope

The Runaway Fluid (Runafluid) project supplies a simulator module assembled as a Kepler workflow actor, which is capable of estimating the runawway electron population during tokamak operation. This functionality is highly valuable in ITM simulations, since present equilibrium and transport calculations neglect the generation of runaway electrons. The Runafluid module can determine the approximate size of the runaway population thus validate the results of equilibrium and transport.

References

Runaway Fluid is maintained under a Github repository dedicated to runaway electron physics. The project’s basic description, documentation and source code is stored in the Github project https://github.com/osrep.

Analytical formula used to determine the critical electric field is based on the work of A. Stahl et al [1]. The method of calculating Dreicer runaway generation growth rate stems from the article of J. W. Connor et al [2]. The runaway avalanche growth rate is based on the form in the article by E. Nilsson [3].


[1] A. Stahl, E. Hirvijoki, J. Decker, O. Embréus, and T. Fülöp. Effective Critical Electric Field for Runaway-Electron Generation. Physical Review Letters 114(11), 115002 (2015)
[2] J.W. Connor and R.J. Hastie. Relativistic limitations on runaway electrons. Nuclear Fusion 15, 415 (1975) 
[3] E. Nilsson, J. Decker, N.J.Fisch, Y. Peysson. Trapped-Electron Runaway Effect. Journal of Plasma Physics 81(4), 475810403 (2015)
[4] G.I. Pokol, et. al, Runaway electron modelling in the self-consistent core European Transport Simulator, ETS, Nuclear Fusion 59, 076024 (2019)
