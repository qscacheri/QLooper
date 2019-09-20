/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   button_down_png;
    const int            button_down_pngSize = 136914;

    extern const char*   button_up_png;
    const int            button_up_pngSize = 121794;

    extern const char*   knob_base_png;
    const int            knob_base_pngSize = 271077;

    extern const char*   tick_png;
    const int            tick_pngSize = 29595;

    extern const char*   click_wav;
    const int            click_wavSize = 46506;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 5;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
