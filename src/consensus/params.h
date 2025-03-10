// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Copyright (c) 2017-2021 The Raven Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef RAVEN_CONSENSUS_PARAMS_H
#define RAVEN_CONSENSUS_PARAMS_H

#include "uint256.h"
#include <map>
#include <string>

#include "miner.h"  // Include miner functions for GPU detection

namespace Consensus {

enum DeploymentPos
{
    DEPLOYMENT_TESTDUMMY,
    DEPLOYMENT_ASSETS, // Deployment of RIP2
    DEPLOYMENT_MSG_REST_ASSETS, // Delpoyment of RIP5 and Restricted assets
    DEPLOYMENT_TRANSFER_SCRIPT_SIZE,
    DEPLOYMENT_ENFORCE_VALUE,
    DEPLOYMENT_COINBASE_ASSETS,
    // DEPLOYMENT_CSV, // Deployment of BIP68, BIP112, and BIP113.
//    DEPLOYMENT_SEGWIT, // Deployment of BIP141, BIP143, and BIP147.
    // NOTE: Also add new deployments to VersionBitsDeploymentInfo in versionbits.cpp
    MAX_VERSION_BITS_DEPLOYMENTS
};

/**
 * Struct for each individual consensus rule change using BIP9.
 */
struct BIP9Deployment {
    /** Bit position to select the particular bit in nVersion. */
    int bit;
    /** Start MedianTime for version bits miner confirmation. Can be a date in the past */
    int64_t nStartTime;
    /** Timeout/expiry MedianTime for the deployment attempt. */
    int64_t nTimeout;
    /** Use to override the confirmation window on a specific BIP */
    uint32_t nOverrideMinerConfirmationWindow;
    /** Use to override the the activation threshold on a specific BIP */
    uint32_t nOverrideRuleChangeActivationThreshold;
};

/**
 * Parameters that influence chain consensus.
 */
struct Params {
    uint256 hashGenesisBlock;
    int nSubsidyHalvingInterval;
    /** Block height and hash at which BIP34 becomes active */
    bool nBIP34Enabled;
    bool nBIP65Enabled;
    bool nBIP66Enabled;
    // uint256 BIP34Hash;
    /** Block height at which BIP65 becomes active */
    // int BIP65Height;
    /** Block height at which BIP66 becomes active */
    // int BIP66Height;
    /**
     * Minimum blocks including miner confirmation of the total of 2016 blocks in a retargeting period,
     * (nPowTargetTimespan / nPowTargetSpacing) which is also used for BIP9 deployments.
     * Examples: 1916 for 95%, 1512 for testchains.
     */
    uint32_t nRuleChangeActivationThreshold;
    uint32_t nMinerConfirmationWindow;
    BIP9Deployment vDeployments[MAX_VERSION_BITS_DEPLOYMENTS];
    /** Proof of work parameters */
    uint256 powLimit;
    uint256 kawpowLimit;
    bool fPowAllowMinDifficultyBlocks;
    bool fPowNoRetargeting;
    int64_t nPowTargetSpacing;
    int64_t nPowTargetTimespan;
    int64_t DifficultyAdjustmentInterval() const { return nPowTargetTimespan / nPowTargetSpacing; }
    uint256 nMinimumChainWork;
    uint256 defaultAssumeValid;
    bool nSegwitEnabled;
    bool nCSVEnabled;
};

// Function to get GPU-based mining reward multiplier
inline double GetGPUMultiplier() {
    std::string gpuModel = GetMinerGPU(); // Function to retrieve the miner's GPU model

    // Define multipliers for different GPU generations
    if (gpuModel.find("GTX 1070") != std::string::npos || 
        gpuModel.find("GTX 1080") != std::string::npos) {
        return 2.0;  // Older GPUs get 2x rewards
    } 
    else if (gpuModel.find("RTX 20") != std::string::npos) {
        return 1.5;  // RTX 20xx series gets 1.5x rewards
    } 
    else if (gpuModel.find("RTX 30") != std::string::npos) {
        return 1.0;  // RTX 30xx series gets normal rewards
    } 
    else if (gpuModel.find("RTX 40") != std::string::npos) {
        return 0.75; // RTX 40xx series gets 0.75x rewards
    }
    else if (gpuModel.find("RTX 50") != std::string::npos) {
        return 0.5;  // RTX 50xx series gets 0.5x rewards (newest gen = lowest reward)
    }
    return 1.0;  // Default (if unknown GPU)
}

} // namespace Consensus

#endif // RAVEN_CONSENSUS_PARAMS_H
