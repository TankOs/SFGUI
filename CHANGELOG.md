# Changelog

## Unreleased

Enhancements:

  * Update SFML version to 3.0.
  * Update CMake version to match SFML's version.

## Release 0.4.0

Enhancements:

  * Modernized CMake configuration.

Fixes:

  * `sfg::Desktop` not properly handling mouse focus between windows.
  * Added missing `<string>` include.
  * Got rid of `SendFakeMouseMoveEvent` warnings.
  * Got rid of `sf::RenderWindow::Create()` deprecation warning.
  * Deactivated Test app features that are broken due to SFML changes.
  * Renamed OpenGL example to avoid a naming collision.
  * _FindSFML.cmake_ updated, libjpeg not required anymore.
  * No `WIN32_LEAN_AND_MEAN` redefinition.

## Release 0.3.2

Enhancements:

  * Compatibility to SFML 2.4.2.
  * Added `sfg::Box::SetOrientation`.

Fixes:

  * Fixed `sfg::Canvas::Draw` `vertex_count` parameter type.

## Release 0.3.1

Enhancements:

  * Implemented support for loading character sets in `sfg::Engine` and
    `sfg::SFGUI` as well. Now, explicitly adding character sets to load will
    suppress the loading of the default character set (codepoint 0x0000 to
    0x0370). The Latin-1 set will have to be explicitly loaded in that case as
    well.
  * Added OnTabChange signal to Notebook.
  * Reuse an already loaded default font object.

Fixes:

  * Fixed `sfg::Separator` not requesting a proper size allocation.
  * Fixed various documentation errors.
  * Fixed `sfg::Frame` requisition calculation.
  * Fixed `sfg::ResourceManager::SetDefaultFont()` assigning the new font to
    the wrong identifier.
  * Fixed `ComboBox::OnSelect` not being emitted when selecting the item which
    is already currently active from the drop-down.
  * Fixed clicking the close button of a window also initiating a drag. (#24)
  * Fixed `Renderer::CreatePane` adding too many new primitives.

## Release 0.3.0

Enhancements:

  * Replaced GLEW with a glLoadGen loader, got rid of GLU dependency.
  * Implemented non-legacy renderer, moved all GL code out of Renderer and into
    the Renderer implementations.
  * Added window close button.
  * Added custom widget example.
  * Added support for getting/setting the SFML view of an sfg::Canvas.
  * Implemented Clear(), Begin() and End() for ComboBoxes (including non-member
    begin() and end() for native support of range-based for loops), renamed
    some ComboBox methods and improved documentation, fixed ComboBox not
    invalidating when elements were removed.
  * Added support for LIB_SUFFIX on Linux.

Fixes:

  * Added GLSL version check to NonLegacyRenderer::IsAvailable().
  * Added missing CMake parameter quoting.
  * Fixed Visual Studio compile error.
  * Fixed crash when using a Notebook with no tabs.
  * Fixed sfg::Canvas not clearing properly.
  * Fixed test example files not being copied properly.
  * Fixed example GUIs not being rendered for a brief period of time when run,
    moved example theme specification blocks before widget creation to reflect
    the recommended sequence of constructing the GUI in order to reduce the
    time required to construct the GUI.
  * Fixed building shared library in Visual Studio due to std::vector being
    specified with an incomplete type, made selecting SFML's link type separate
    from selecting SFGUI's link type, renamed "test" example to "SFGUI-Test" in
    order to fix a CMake policy warning.
  * Fixed errors compiling with Visual Studio, fixed warnings about
    struct/class forward declaration mismatches, got rid of empty
    ResourceLoader translation unit.
  * Fixed broken GLX linking.
  * Fixed glX symbols not being found because was X11 not being linked on
    Linux, rewrote the FindSFGUI.cmake script.
  * Fixed Scrollbars not calculating their slider size properly based on the
    adjustment values, fixed ScrolledWindow sometimes displaying 2 scrollbars
    instead of only 1 when both are set to automatic and the child viewport is
    expanded
  * Fixed SFGUI crashing when connecting a widget to an adjustment and letting
    the widget get destroyed before the adjustment.
  * Fixed ScrolledWindow not causing child scrollbars to resize properly,
    prevented being able to overwrite ScrolledWindow viewport with another even
    if it isn't added to the container.
  * Fixed ComboBox trying to select entries with a negative index
  * Added quotes to CMakeLists.txts where strings with spaces might occur
  * Fixed static linking and added support for static runtime library.
  * Fixed clang compiler flags.
  * Fixed compilation with VS 2015.
  * Fixed rendering glitches when viewport is resized.

## Release 0.2.3

Fixes:

  * Now using both GLEW_INCLUDE_PATH and GLEW_INCLUDE_DIRS for SFML
    compatibility reasons.

## Release 0.2.2

Enhancements:

  * Added possibility for specifying optional character sets for the renderer
    during runtime to preload glyphs.

Fixes:

  * Replaced GLEW_INCLUDE_PATH by GLEW_INCLUDE_DIRS in CMake config.
  * Added missing override declarations.

## Release 0.2.1

Enhancements:

  * GLee replaced by GLEW.

Fixes:

  * Fixed compilation error on GLXContextID.
  * Fixed CMake configuration generating library file names that ld couldn't
    find when using symbolic names during linking.

## Release 0.2.0

Enhancements:

  * Conversion of codebase to use C++11 features.
  * Implemented two backend renderers (one uses vertex buffer objects, the
    other client-side vertex arrays).
  * Generalized renderer textured quad creation from CreateImage into
    CreateSprite allowing Engine developers more freedom to manage resources
    and convey their usage.
  * Allow creation of RadioButtonGroups in advance.

Fixes:

  * Fixed linking and general building errors on OSX.
  * Fixed corner case bug with canvas.
  * Fixed viewports not propagating properly.
  * Fixed index count not being reduced in renderer when primitives are
    removed.
  * Fixed FindSFGUI.cmake ignoring standard arguments.
  * Removed obsolete UpdateViewRect() from sfg::Desktop.
  * Fixed renderer crashing when drawing without any textures loaded into the
    atlas.
  * Fixed loading fonts in renderer.
  * Adapted CMake configuration to the new SFML linking system.
  * Got rid of SFML as an extlib (Git submodule).
  * Cleaned up CMake configuration file.

## Release 0.1.1

Enhancements:

  * Compatibility to SFML 2.1.
  * Reimplemented sfg::Canvas.
  * Rewrote sfg::Renderer::Display() interface to allow sf::Windows to be used
    as targets for GUI rendering as well.
  * Added support for multiple pages in the texture atlas to support
    arbitrarily many images and image sizes (that themselves would fit on the
    GPU of course)
  * Added PackStart, PackEnd and ReorderChild to Box to make using it easier.

Fixes:

  * Made sure to restore coherency between wiped states cache and OpenGL
    texture binding by unbinding any bound textures after wiping the cache.
  * Moved spinner RenderQueue update logic into Engine-side code to decouple
    the Widget from it's Engine implementation and make it easier for custom
    Engines to be written for it.
  * Changed CMakeLists.txt to accept SFML 2.x instead of only 2.0.
  * Fixed hiding ACTIVE widgets getting them stuck in ACTIVE state, blocking
    all future events from reaching other widgets.
  * Fixed appending and prepending text to an Entry not properly invalidating
    it.
  * Fixed removing child from table not removing associated cell entry.
  * Changed found message type in FindSFGUI.cmake to STATUS to prevent people
    from being unnecessarily surprised when something worked. #636
  * Fixed clicking on combobox drop-down menu causing window reordering when
    the menu reached over the edge of the parent window onto another window.
    The Desktop now only considers reordering when there is no modal widget
    active. #628
  * Fixed Window not properly ignoring invalid mouse move values. #607
  * Fixed m_parent_notified being uninitialised on first use in Widget. #638
  * Added missing "out of window" check when handling MouseMove events in
    sfg::Scale.
  * Fixed not taking into account padding when determining if the Renderer
    needs to create a new atlas page.
  * Fixed child widgets not notifying tentative parents of the child's
    invalidation if the invalidation took place before it was added to the
    parent and the next update cycle.
  * Added explicit sf::Context construction before querying GLee for supported
    extensions to fix special cases where no sf::Context is available yet thus
    making GLee report wrong extension information.
  * Fixed broken behavior when removing a page from a Notebook, now warns when
    trying to remove the last page of the notebook

## Release 0.1.0

Enhancements:

  * Implemented SpinButton widget.
  * Optimized primitive construction and label resizing.
  * Linking to dependencies skipped for Linux systems.
  * Removed (too) specific settings regarding the linker (no more linking to
    static runtime libraries and SFML static linking settings).
  * Examples' data directory copied to CMake build cache directory for easier
    testing when developing.
  * SFGUI_ROOT is now the preferred way of given an extra search path when
    looking for SFGUI through find_package(). The old SFGUIDIR is still
    available.

Fixes:

  * Mouse leaving SFML window causing a very very long scroll action with
    scrollbars, resulting in lag.
  * Uninitialized text margin variable in entry widget.
  * Widgets not automatically refreshing when a property is changed after their
    drawables are constructed.
  * Widgets not automatically refreshing when their ID or class is changed.
  * sf::Event::MouseEntered and sf::Event::MouseLeft not being handled
    properly.

## Release 0.0.1

  * FIX: Compatible to latest SFML commit.

## Release 0.0.0

  * First release.
