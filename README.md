# Project Cafeberry
Cafeberry is a project aiming to update the December 17th 2014 codebase (mostly) via decompilation, with a focus on consoles.

[![Discord](https://img.shields.io/badge/Discord-Join%20Server-5865F2?logo=discord&logoColor=white)](https://discord.gg/aPAF5WFaW)

# Status
| Platform       | Development Status    | Known Issues                                                              |
| :------------- | :-------------------- | :------------------------------------------------------------------------ |
| PS3            | Active                | SPU variants for TU19 blocks are missing, needs to be further optimized.  |
| Xbox 360       | Active                | Works, Crashes when using updated XUI Scenes. See Notes                   |
| PS4            | Active                | Only Easily buildable on Windows 7. See Notes                             |
| Xbox One       | On hold               | Needs to be re-coded to run on Developer Mode UWP instead.                |
| PSVita         | Active                | Buildable With 3.570 or 3.750 SDK.                                        |
| Wii U & Switch | Work in progress      | UI System (ImXui) is still in progress, need to decompile the rest.       |

**Notes:**
- **PS4:** Can't be built on Windows 10+ because the 1.70 SDK only supports Windows 7, solutions:
  1. Attempt to add support for 4.50 SDK (difficult or unlikely to work)
  2. Force the 1.70 SDK on Windows 10+ (Takes 5-15m)
- **Xbox 360:** Needs XUI Scenes from TU18+, solutions:
  1. Get XURS from built .xex's and turn them back into XUI file's
  2. Recreate XUI scenes based off of Iggy scenes from TU57+ 