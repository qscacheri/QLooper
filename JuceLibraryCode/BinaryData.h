/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   background_png;
    const int            background_pngSize = 882332;

    extern const char*   button_off_down_png;
    const int            button_off_down_pngSize = 139232;

    extern const char*   button_off_up_png;
    const int            button_off_up_pngSize = 123043;

    extern const char*   button_on_down_png;
    const int            button_on_down_pngSize = 138854;

    extern const char*   button_on_up_png;
    const int            button_on_up_pngSize = 122993;

    extern const char*   divider_png;
    const int            divider_pngSize = 12265;

    extern const char*   knob_base_png;
    const int            knob_base_pngSize = 247007;

    extern const char*   metronome_button_png;
    const int            metronome_button_pngSize = 140761;

    extern const char*   tick_png;
    const int            tick_pngSize = 48374;

    extern const char*   click_wav;
    const int            click_wavSize = 46506;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 10;

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
