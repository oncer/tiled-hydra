.. raw:: html

   <div class="new">New in Tiled 1.3</div>

Scripting
=========

Introduction
------------

Initial scripting capabilities have been added to Tiled. The API is still
incomplete, but many actions can be automated either by interacting with any
open assets or by triggering UI actions.

Scripts can be used to implement :ref:`map export formats <script-registerMapFormat>`
(only text formats for now), :ref:`custom actions <script-registerAction>` and
:ref:`new tools <script-registerTool>`.

On startup, Tiled will execute any script files present in
:ref:`extensions <script-extensions>`. In addition it is possible to run
scripts directly from :ref:`the console <script-console>`. All scripts share
a single JavaScript context.

.. _script-extensions:

Scripted Extensions
^^^^^^^^^^^^^^^^^^^

Extensions are placed in a system-specific location. This folder can be opened
from the Plugins tab in the Preferences dialog.

+-------------+-----------------------------------------------------------------+
| **Windows** | | :file:`C:/Users/<USER>/AppData/Local/Tiled/extensions/`       |
+-------------+-----------------------------------------------------------------+
| **macOS**   | | :file:`~/Library/Preferences/Tiled/extensions/`               |
+-------------+-----------------------------------------------------------------+
| **Linux**   | | :file:`~/.config/tiled/extensions/`                           |
+-------------+-----------------------------------------------------------------+

Each extension is expected to be placed in a sub-directory of the extensions
directory. All scripts files found in these sub-directories are executed on
startup.

.. note::

    If your scripts depend on other scripts that you want to include rather
    than have them execute directly, they can be nested in another
    sub-directory.

When any loaded script is changed, the script engine is reinstantiated and the
scripts are reloaded. This makes it quick to iterate on a script until it
works as intended.

Apart from scripts, extensions can include images that can be used as the icon
for scripted actions or tools.

Startup Script
^^^^^^^^^^^^^^

.. warning::

    This functionality is deprecated. Write an
    :ref:`extension <script-extensions>` instead.

If present, a :file:`startup.js` script is evaluated on startup. This
script could define functions that can be called from the Console or can
connect to signals to add functionality.

The location of the startup script depends on the platform. The file
:file:`startup.js` is searched for in the following locations:

+-------------+-----------------------------------------------------------------+
| **Windows** | | :file:`C:/Users/<USER>/AppData/Local/Tiled/startup.js`        |
|             | | :file:`C:/ProgramData/Tiled/startup.js`                       |
+-------------+-----------------------------------------------------------------+
| **macOS**   | | :file:`~/Library/Preferences/Tiled/startup.js`                |
+-------------+-----------------------------------------------------------------+
| **Linux**   | | :file:`~/.config/tiled/startup.js`                            |
|             | | :file:`/etc/xdg/tiled/startup.js`                             |
+-------------+-----------------------------------------------------------------+

Any file that exists will be evaluated.

As with extensions, the script engine is reinstantiated and the scripts are
reloaded when the startup script is changed.

.. _script-console:

Console View
^^^^^^^^^^^^

In the Console view (*View > Views and Toolbars > Console*) you will
find a text entry where you can write or paste scripts to evaluate them.

You can use the Up/Down keys to navigate through previously entered
script expressions.

Connecting to Signals
^^^^^^^^^^^^^^^^^^^^^

The script API provides signals to which functions can be connected.
Currently, the tiled module has the most useful :ref:`set of signals <script-tiled-signals>`.

Properties usually will have related signals which can be used to detect
changes to that property, but most of those are currently not
implemented.

To connect to a signal, call its ``connect`` function and pass in a
function object. In the following example, newly created maps
automatically get their first tile layer removed:

.. code:: js

    tiled.assetCreated.connect(function(asset) {
        if (asset.layerCount > 0) {
            asset.removeLayerAt(0)
            tiled.log("assetCreated: Removed automatically added tile layer.")
        }
    })

In some cases it will be necessary to later disconnect the function from
the signal again. This can be done by defining the function separately
and passing it into the ``disconnect`` function:

.. code:: js

    function onAssetCreated(asset) {
        // Do something...
    }

    tiled.assetCreated.connect(onAssetCreated)
    // ...
    tiled.assetCreated.disconnect(onAssetCreated)

API Reference
-------------

tiled module
^^^^^^^^^^^^

The ``tiled`` module is the main entry point and provides properties,
functions and signals which are documented below.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **version** : string |ro|, Currently used version of Tiled.
    **platform** : string |ro|, "Operating system. One of ``windows``, ``macos``, ``linux`` or ``unix``
    (for any other UNIX-like system)."
    **arch** : string |ro|, "Processor architecture. One of ``x64``, ``x86`` or ``unknown``."
    **actions** : [string] |ro|, "Available actions for :ref:`tiled.trigger() <script-trigger>`."
    **menus** : [string] |ro|, "Available menus for :ref:`tiled.extendMenu() <script-extendMenu>`."
    **activeAsset** : :ref:`script-asset`, "Currently selected asset, or ``null`` if no file is open. Can be assigned
    any open asset in order to change the active asset."
    **openAssets** : array |ro|, "List of currently opened :ref:`assets <script-asset>`."
    **tilesetEditor** : :ref:`script-tileseteditor`, "Access the editor used when editing tilesets."

Functions
~~~~~~~~~

.. _script-trigger:

tiled.trigger(action : string) : void
    This function can be used to trigger any registered action. This
    includes most actions you would normally trigger through the menu or by
    using their shortcut.

    Use the ``tiled.actions`` property to get a list of all available actions.

    Actions that are checkable will toggle when triggered.

.. _script-execute:

tiled.executeCommand(name : string, inTerminal : bool) : bool
    Executes the first custom command with the given name, as if it was
    triggered manually. Works also with commands that are not currently enabled.

.. _script-open:

tiled.open(fileName : string) : :ref:`script-asset`
    Requests to open the asset with the given file name. Returns a reference to
    the opened asset, or ``null`` in case there was a problem.

.. _script-close:

tiled.close(asset : :ref:`script-asset`) : bool
    Closes the given asset without checking for unsaved changes (to confirm the
    loss of any unsaved changes, set ``activeAsset`` and trigger the "Close"
    action instead).

.. _script-reload:

tiled.reload(asset : :ref:`script-asset`) : :ref:`script-asset`
    Reloads the given asset from disk, without checking for unsaved changes.
    This invalidates the previous script reference to the asset, hence the new
    reference is returned for convenience. Returns ``null`` if reloading failed.

tiled.alert(text : string [, title : string]) : void
    Shows a modal warning dialog to the user with the given text and
    optional title.

tiled.confirm(text : string [, title : string]) : bool
    Shows a yes/no dialog to the user with the given text and optional
    title. Returns ``true`` or ``false``.

tiled.prompt(label : string [, text : string [, title : string]]) : string
    Shows a dialog that asks the user to enter some text, along with the
    given label and optional title. The optional ``text`` parameter provides
    the initial value of the text. Returns the entered text.

tiled.log(text : string) : void
    Outputs the given text in the Console window as regular text.

tiled.warn(text : string, activated : function) : void
    Outputs the given text in the Console window as warning message and creates
    an issue in the Issues window.

    When the issue is activated (with double-click or Enter key) the given
    callback function is invoked.

tiled.error(text : string, activated : function) : void
    Outputs the given text in the Console window as error message and creates
    an issue in the Issues window.

    When the issue is activated (with double-click or Enter key) the given
    callback function is invoked.

.. _script-registerAction:

tiled.registerAction(id : string, callback : function) : :ref:`script-action`
    Registers a new action with the given ``id`` and ``callback`` (which is
    called when the action is triggered). The returned action object can be
    used to set (and update) various properties of the action.

    Example:

    .. code:: javascript

        var action = tiled.registerAction("CustomAction", function(action) {
            tiled.log(action.text + " was " + (action.checked ? "checked" : "unchecked"))
        })

        action.text = "My Custom Action"
        action.checkable = true
        action.shortcut = "Ctrl+K"

    The shortcut will currently only work when the action is added to a menu
    using :ref:`tiled.extendMenu() <script-extendMenu>`.

.. _script-registerMapFormat:

tiled.registerMapFormat(shortName : string, mapFormat : object) : void
    Registers a new map format that can then be used to export maps to.

    If a map format is already registered with the same ``shortName``,
    the existing format is replaced. The short name can also be used to
    specify the format when using ``--export-map`` on the command-line,
    in case the file extension is ambiguous or a different one should be
    used.

    The ``mapFormat`` object is expected to have the following properties:

    .. csv-table::
        :widths: 1, 2

        **name** : string, Name of the format as shown in the file dialog.
        **extension** : string, The file extension used by the format.
        "**toString** : function(map : :ref:`script-map`, fileName : string) : string", "A function
        that returns the string representation of the given map, when
        saved to the given file name (useful for relative references)."

    Example that produces a simple JSON representation of a map:

    .. code:: javascript

        var customMapFormat = {
            name: "Custom map format",
            extension: "custom",

            toString: function(map, fileName) {
                var m = {
                    width: map.width,
                    height: map.height,
                    layers: []
                };

                for (var i = 0; i < map.layerCount; ++i) {
                    var layer = map.layerAt(i);
                    if (layer.isTileLayer) {
                        var rows = [];
                        for (y = 0; y < layer.height; ++y) {
                            var row = [];
                            for (x = 0; x < layer.width; ++x)
                                row.push(layer.cellAt(x, y).tileId);
                            rows.push(row);
                        }
                        m.layers.push(rows);
                    }
                }

                return JSON.stringify(m);
            },
        }

        tiled.registerMapFormat("custom", customMapFormat)

.. _script-registerTool:

tiled.registerTool(shortName : string, tool : object) : object
    Registers a custom tool that will become available on the Tools tool bar
    of the Map Editor.

    If a tool is already registered with the same ``shortName`` the existing
    tool is replaced.

    The ``tool`` object has the following properties:

    .. csv-table::
        :widths: 1, 2

        **name** : string, Name of the tool as shown on the tool bar.
        **map** : :ref:`script-map`, Currently active tile map.
        **selectedTile** : :ref:`script-tile`, The last clicked tile for the active map.
        **tilePosition** : :ref:`script-point`, Mouse cursor position in tile coordinates.
        **statusInfo** : string, Text shown in the status bar while the tool is active.
        **enabled** : bool, Whether this tool is enabled.
        "**activated** : function() : void", Called when the tool was activated.
        "**deactivated** : function() : void", Called when the tool was deactivated.
        "**keyPressed** : function(key, modifiers) : void", Called when a key was pressed while the tool was active.
        "**mouseEntered** : function() : void", Called when the mouse entered the map view.
        "**mouseLeft** : function() : void", Called when the mouse left the map view.
        "**mouseMoved** : function(x, y, modifiers) : void", Called when the mouse position in the map scene changed.
        "**mousePressed** : function(button, x, y, modifiers) : void", Called when a mouse button was pressed.
        "**mouseReleased** : function(button, x, y, modifers) : void", Called when a mouse button was released.
        "**mouseDoubleClicked** : function(button, x, y, modifiers) : void", Called when a mouse button was double-clicked.
        "**modifiersChanged** : function(modifiers) : void", Called when the active modifier keys changed.
        "**languageChanged** : function() : void", Called when the language was changed.
        "**mapChanged** : function(oldMap : :ref:`script-map`, newMap : :ref:`script-map`) : void", Called when the active map was changed.
        "**tilePositionChanged** : function() : void", Called when the hovered tile position changed.
        "**updateStatusInfo** : function() : void", Called when the hovered tile position changed. Used to override the default updating of the status bar text.
        "**updateEnabledState** : function() : void", Called when the map or the current layer changed.

    Here is an example tool that places a rectangle each time the mouse has
    moved by 32 pixels:

    .. code:: javascript

        var tool = tiled.registerTool("PlaceRectangles", {
            name: "Place Rectangles",

            mouseMoved: function(x, y, modifiers) {
                if (!this.pressed)
                    return

                var dx = Math.abs(this.x - x)
                var dy = Math.abs(this.y - y)

                this.distance += Math.sqrt(dx*dx + dy*dy)
                this.x = x
                this.y = y

                if (this.distance > 32) {
                    var objectLayer = this.map.currentLayer

                    if (objectLayer && objectLayer.isObjectLayer) {
                        var object = new MapObject(++this.counter)
                        object.x = Math.min(this.lastX, x)
                        object.y = Math.min(this.lastY, y)
                        object.width = Math.abs(this.lastX - x)
                        object.height = Math.abs(this.lastY - y)
                        objectLayer.addObject(object)
                    }

                    this.distance = 0
                    this.lastX = x
                    this.lastY = y
                }
            },

            mousePressed: function(button, x, y, modifiers) {
                this.pressed = true
                this.x = x
                this.y = y
                this.distance = 0
                this.counter = 0
                this.lastX = x
                this.lastY = y
            },

            mouseReleased: function(button, x, y, modifiers) {
                this.pressed = false
            },
        })


.. _script-extendMenu:

tiled.extendMenu(id : string, items : array | object) : void
    Extends the menu with the given ID. Supports both a list of items or a
    single item. Available menu IDs can be obtained using the ``tiled.menus``
    property.

    A menu item is defined by an object with the following properties:

    .. csv-table::
        :widths: 1, 2

        **action** : string, ID of a registered action that the menu item will represent.
        **before** : string, ID of the action before which this menu item should be added (optional).
        **separator** : bool, Set to ``true`` if this item is a menu separator (optional).

    If a menu item does not include a ``before`` property, the value is
    inherited from the previous item. When this property is not set at all,
    the items are appended to the end of the menu.

    Example that adds a custom action to the "Edit" menu, before the "Select
    All" action and separated by a separator:

    .. code:: javascript

        tiled.extendMenu("Edit", [
            { action: "CustomAction", before: "SelectAll" },
            { separator: true }
        ]);

    The "CustomAction" will need to have been registered before using
    :ref:`tiled.registerAction() <script-registerAction>`.

.. _script-tiled-signals:

Signals
~~~~~~~

tiled.assetCreated(asset : :ref:`script-asset`)
    A new asset has been created.

tiled.assetOpened(asset : :ref:`script-asset`)
    An asset has been opened.

tiled.assetAboutToBeSaved(asset : :ref:`script-asset`)
    An asset is about to be saved. Can be used to make last-minute changes.

tiled.assetSaved(asset : :ref:`script-asset`)
    An asset has been saved.

tiled.assetAboutToBeClosed(asset : :ref:`script-asset`)
    An asset is about to be closed.

tiled.activeAssetChanged(asset : :ref:`script-asset`)
    The currently active asset has changed.


.. _script-object:

Object
^^^^^^

The base of most data types in Tiled. Provides the ability to associate custom
properties with the data.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **asset** : :ref:`script-asset` |ro|, "The asset this object is part of, or ``null``."
    **readOnly** : bool |ro|, Whether the object is read-only.

Functions
~~~~~~~~~

.. _script-object-property:

Object.property(name : string) : variant
    Returns the value of the custom property with the given name, or
    ``undefined`` if no such property is set on the object.

    *Note:* Currently it is not possible to inspect the value of ``file`` properties.

.. _script-object-setProperty:

Object.setProperty(name : string, value : variant) : void
    Sets the value of the custom property with the given name. Supported types
    are ``bool``, ``number`` and ``string``. When setting a ``number``, the
    property type will be set to either ``int`` or ``float``, depending on
    whether it is a whole number.

    *Note:* Support for ``color`` and ``file`` properties is currently missing.

.. _script-object-properties:

Object.properties() : object
    Returns all custom properties set on this object. Modifications to the
    properties will not affect the original object.

.. _script-object-setProperties:

Object.setProperties(properties : object) : void
    Replaces all currently set custom properties with a new set of properties.

.. _script-object-removeProperty:

Object.removeProperty(name : string) : void
    Removes the custom property with the given name.

.. _script-asset:

Asset
^^^^^

Inherits :ref:`script-object`.

Represents any top-level data type that can be saved to a file. Currently
either a :ref:`script-map` or a :ref:`script-tileset`.

All modifications made to assets and their contained parts create undo
commands. This includes both modifying functions that are called as well as
simply assigning to a writable property.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **fileName** : string |ro|, File name of the asset.
    **modified** : bool |ro|, Whether the asset was modified after it was saved or loaded.
    **isTileMap** : bool |ro|, Whether the asset is a :ref:`script-map`.
    **isTileset** : bool |ro|, Whether the asset is a :ref:`script-tileset`.

Functions
~~~~~~~~~

Asset.macro(text : string, callback : function) : value
    Creates a single undo command that wraps all changes applied to this asset
    by the given callback. Recommended to avoid spamming the undo stack with
    small steps that the user does not care about.

    Example function that changes visibility of multiple layers in one step:

    .. code:: javascript

        tileMap.macro((visible ? "Show" : "Hide") + " Selected Layers", function() {
            tileMap.selectedLayers.forEach(function(layer) {
                layer.visible = visible
            })
        })

    The returned value is whatever the callback function returned.

.. _script-map:

TileMap
^^^^^^^

Inherits :ref:`script-asset`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **width** : int |ro|, Width of the map in tiles (only relevant for non-infinite maps). Use :ref:`resize <script-map-resize>` to change it.
    **height** : int |ro|, Height of the map in tiles (only relevant for non-infinite maps). Use :ref:`resize <script-map-resize>` to change it.
    **size** : size |ro|, Size of the map in tiles (only relevant for non-infinite maps). Use :ref:`resize <script-map-resize>` to change it.
    **tileWidth** : int, Tile width (used by tile layers).
    **tileHeight**: int, Tile height (used by tile layers).
    **infinite** : bool, Whether this map is infinite.
    **hexSideLength** : int, Length of the side of a hexagonal tile (used by tile layers on hexagonal maps).
    **staggerAxis** : :ref:`StaggerAxis <script-map-staggeraxis>`, "For staggered and hexagonal maps, determines which axis (X or Y) is staggered."
    **orientation** : :ref:`Orientation <script-map-orientation>`, "General map orientation"
    **renderOrder** : :ref:`RenderOrder <script-map-renderorder>`, "Tile rendering order (only implemented for orthogonal maps)"
    **staggerIndex** : :ref:`StaggerIndex <script-map-staggerindex>`, "For staggered and hexagonal maps, determines whether the even or odd indexes along the staggered axis are shifted."
    **backgroundColor** : color, Background color of the map.
    **layerDataFormat** : :ref:`LayerDataFormat <script-map-layerdataformat>`, "The format in which the layer data is stored, taken into account by TMX, JSON and Lua map formats."
    **layerCount** : int |ro|, Number of top-level layers the map has.
    **tilesets** : [:ref:`script-tileset`], "The list of tilesets referenced by this map. To determine which tilesets are actually used, call :ref:`usedTilesets() <script-map-usedTilesets>`."
    **selectedArea** : :ref:`SelectionArea <script-selectedarea>`, The selected area of tiles.
    **currentLayer** : :ref:`script-layer`, The current layer.
    **selectedLayers** : [:ref:`script-layer`], Selected layers.
    **selectedObjects** : [:ref:`script-mapobject`], Selected objects.

.. _script-map-orientation:

.. csv-table::
    :header: "TileMap.Orientation"

    TileMap.Unknown
    TileMap.Orthogonal
    TileMap.Isometric
    TileMap.Staggered
    TileMap.Hexagonal

.. _script-map-layerdataformat:

.. csv-table::
    :header: "TileMap.LayerDataFormat"

    TileMap.XML
    TileMap.Base64
    TileMap.Base64Gzip
    TileMap.Base64Zlib
    TileMap.Base64Zstandard
    TileMap.CSV

.. _script-map-renderorder:

.. csv-table::
    :header: "TileMap.RenderOrder"

    TileMap.RightDown
    TileMap.RightUp
    TileMap.LeftDown
    TileMap.LeftUp

.. _script-map-staggeraxis:

.. csv-table::
    :header: "TileMap.StaggerAxis"

    TileMap.StaggerX
    TileMap.StaggerY

.. _script-map-staggerindex:

.. csv-table::
    :header: "TileMap.StaggerIndex"

    TileMap.StaggerOdd
    TileMap.StaggerEven

Functions
~~~~~~~~~

new TileMap()
    Constructs a new map.

.. _script-map-layerAt:

TileMap.layerAt(index : int) : :ref:`script-layer`
    Returns a reference to the top-level layer at the given index. When the
    layer gets removed from the map, the reference changes to a standalone
    copy of the layer.

.. _script-map-removeLayerAt:

TileMap.removeLayerAt(index : int) : void
    Removes the top-level layer at the given index. When a reference to the
    layer still exists, that reference becomes a standalone copy of the layer.

.. _script-map-removeLayer:

TileMap.removeLayer(layer : :ref:`script-layer`) : void
    Removes the given layer from the map. The reference to the layer becomes
    a standalone copy.

.. _script-map-insertLayerAt:

TileMap.insertLayerAt(index : int, layer : :ref:`script-layer`) : void
    Inserts the layer at the given index. The layer can't already be part of
    a map.

.. _script-map-addLayer:

TileMap.addLayer(layer : :ref:`script-layer`) : void
    Adds the layer to the map, above all existing layers. The layer can't
    already be part of a map.

.. _script-map-addTileset:

TileMap.addTileset(tileset : :ref:`script-tileset`) : bool
    Adds the given tileset to the list of tilesets referenced by this map.
    Returns ``true`` if the tileset was added, or ``false`` if the tileset was
    already referenced by this map.

.. _script-map-replaceTileset:

TileMap.replaceTileset(oldTileset : :ref:`script-tileset`, newTileset : :ref:`script-tileset`) : bool
    Replaces all occurrences of ``oldTileset`` with ``newTileset``. Returns
    ``true`` on success, or ``false`` when either the old tileset was not
    referenced by the map, or when the new tileset was already referenced by
    the map.

.. _script-map-removeTileset:

TileMap.removeTileset(tileset : :ref:`script-tileset`) : bool
    Removes the given tileset from the list of tilesets referenced by this
    map. Returns ``true`` on success, or ``false`` when the given tileset was
    not referenced by this map or when the tileset was still in use by a tile
    layer or tile object.

.. _script-map-usedTilesets:

TileMap.usedTilesets() : [:ref:`script-tileset`]
    Returns the list of tilesets actually used by this map. This is generally
    a subset of the tilesets referenced by the map (the ``TileMap.tilesets``
    property).

.. _script-map-resize:

TileMap.resize(size : size [, offset : :ref:`script-point` [, removeObjects : bool = false]]) : void
    Resizes the map to the given size, optionally applying an offset (in tiles)

.. _script-layer:

Layer
^^^^^

Inherits :ref:`script-object`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **name** : string, Name of the layer.
    **opacity** : number, "Opacity of the layer, from 0 (fully transparent) to 1 (fully opaque)."
    **visible** : bool, Whether the layer is visible (affects child layer visibility for group layers).
    **locked** : bool, Whether the layer is locked (affects whether child layers are locked for group layers).
    **offset** : :ref:`script-point`, Offset in pixels that is applied when this layer is rendered.
    **map** : :ref:`script-map`, Map that this layer is part of (or ``null`` in case of a standalone layer).
    **selected** : bool, Whether the layer is selected.
    **isTileLayer** : bool |ro|, Whether this layer is a :ref:`script-tilelayer`.
    **isObjectGroup** : bool |ro|, Whether this layer is an :ref:`script-objectgroup`.
    **isGroupLayer** : bool |ro|, Whether this layer is a :ref:`script-grouplayer`.
    **isImageLayer** : bool |ro|, Whether this layer is an :ref:`script-imagelayer`.

.. _script-tilelayer:

TileLayer
^^^^^^^^^

Inherits :ref:`script-layer`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **width** : int |ro|, Width of the layer in tiles (only relevant for non-infinite maps).
    **height** : int |ro|, Height of the layer in tiles (only relevant for non-infinite maps).
    **size** : size |ro|, Size of the layer in tiles (has ``width`` and ``height`` members) (only relevant for non-infinite maps).

Functions
~~~~~~~~~

new TileLayer([name : string])
    Constructs a new tile layer, which can be added to a :ref:`script-map`.

TileLayer.region() : region
    Returns the region of the layer that is covered with tiles.

TileLayer.cellAt(x : int, y : int) : cell
    Returns the value of the cell at the given position. Can be used to query
    the flags and the tile ID, but does not currently allow getting a tile
    reference.

TileLayer.flagsAt(x : int, y : int) : int
    Returns the :ref:`flags <script-tile-flags>` used for the tile at the given
    position.

TileLayer.tileAt(x : int, y : int) : :ref:`script-tile`
    Returns the tile used at the given position, or ``null`` for empty spaces.

.. _script-tilelayer-edit:

TileLayer.edit() : :ref:`script-tilelayeredit`
    Returns an object that enables making modifications to the tile layer.

.. _script-objectgroup:

ObjectGroup
^^^^^^^^^^^

Inherits :ref:`script-layer`.

The "ObjectGroup" is a type of layer that can contain objects. It will
henceforth be referred to as a layer.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **objects** : [:ref:`script-mapobject`] |ro|, Array of all objects on this layer.
    **objectCount** : int |ro|, Number of objects on this layer.
    **color** : color, Color of shape and point objects on this layer (when not set by object type).

Functions
~~~~~~~~~

new ObjectGroup([name : string])
    Constructs a new object layer, which can be added to a :ref:`script-map`.

ObjectGroup.objectAt(index : int) : :ref:`script-mapobject`
    Returns a reference to the object at the given index. When the object is
    removed, the reference turns into a standalone copy of the object.

ObjectGroup.removeObjectAt(index : int) : void
    Removes the object at the given index.

ObjectGroup.removeObject(object : :ref:`script-mapobject`) : void
    Removes the given object from this layer. The object reference turns into
    a standalone copy of the object.

ObjectGroup.insertObjectAt(index : int, object : :ref:`script-mapobject`) : void
    Inserts the object at the given index. The object can't already be part
    of a layer.

ObjectGroup.addObject(object : :ref:`script-mapobject`) : void
    Adds the given object to the layer. The object can't already be part of
    a layer.

.. _script-grouplayer:

GroupLayer
^^^^^^^^^^

Inherits :ref:`script-layer`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **layerCount** : int |ro|, Number of child layers the group layer has.

Functions
~~~~~~~~~

new GroupLayer([name : string])
    Constructs a new group layer.

GroupLayer.layerAt(index : int) : :ref:`script-layer`
    Returns a reference to the child layer at the given index.

GroupLayer.removeLayerAt(index : int) : void
    Removes the child layer at the given index. When a reference to the layer
    still exists and this group layer isn't already standalone, that reference
    becomes a standalone copy of the layer.

GroupLayer.removeLayer(layer : :ref:`script-layer`) : void
    Removes the given layer from the group. If this group wasn't standalone,
    the reference to the layer becomes a standalone copy.

GroupLayer.insertLayerAt(index : int, layer : :ref:`script-layer`) : void
    Inserts the layer at the given index. The layer can't already be part of
    a map.

GroupLayer.addLayer(layer : :ref:`script-layer`) : void
    Adds the layer to the group, above all existing layers. The layer can't
    already be part of a map.

.. _script-imagelayer:

ImageLayer
^^^^^^^^^^

Inherits :ref:`script-layer`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **transparentColor** : color, Color used as transparent color when rendering the image.
    **imageSource** : url, Reference to the image rendered by this layer.

.. _script-mapobject:

MapObject
^^^^^^^^^

Inherits :ref:`script-object`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **id** : int |ro|, Unique (map-wide) ID of the object.
    **shape** : int, :ref:`Shape <script-mapobject-shape>` of the object.
    **name** : string, Name of the object.
    **type** : string, Type of the object.
    **x** : number, X coordinate of the object in pixels.
    **y** : number, Y coordinate of the object in pixels.
    **pos** : :ref:`script-point`, Position of the object in pixels.
    **width** : number, Width of the object in pixels.
    **height** : number, Height of the object in pixels.
    **size** : size, Size of the object in pixels (has ``width`` and ``height`` members).
    **rotation** : number, Rotation of the object in degrees clockwise.
    **visible** : bool, Whether the object is visible.
    **polygon** : :ref:`Polygon <script-polygon>`, Polygon of the object.
    **text** : string, The text of a text object.
    **font** : :ref:`script-font`, The font of a text object.
    **textAlignment** : :ref:`script-alignment`, The alignment of a text object.
    **wordWrap** : bool, Whether the text of a text object wraps based on the width of the object.
    **textColor** : color, Color of a text object.
    **tile** : :ref:`script-tile`, Tile of the object.
    **tileFlippedHorizontally** : bool, Whether the tile is flipped horizontally.
    **tileFlippedVertically** : bool, Whether the tile is flipped vertically.
    **selected** : bool, Whether the object is selected.
    **layer** : :ref:`script-objectgroup` |ro|, Layer this object is part of (or ``null`` in case of a standalone object).
    **map** : :ref:`script-map` |ro|, Map this object is part of (or ``null`` in case of a standalone object).

.. _script-mapobject-shape:

.. csv-table::
    :header: "MapObject.Shape"

    MapObject.Rectangle
    MapObject.Polygon
    MapObject.Polyline
    MapObject.Ellipse
    MapObject.Text
    MapObject.Point

Functions
~~~~~~~~~

new MapObject([name : string])
    Constructs a new map object, which can be added to an :ref:`script-objectgroup`.

.. _script-tileset:

Tileset
^^^^^^^

Inherits :ref:`script-asset`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **name** : string, Name of the tileset.
    **tiles**: [:ref:`script-tile`] |ro|, Array of all tiles in this tileset. Note that the index of a tile in this array does not always match with its ID.
    **terrains**: [:ref:`script-terrain`] |ro|, Array of all terrains in this tileset.
    **tileCount** : int, The number of tiles in this tileset.
    **tileWidth** : int |ro|, Tile width for tiles in this tileset in pixels.
    **tileHeight** : int |ro|, Tile Height for tiles in this tileset in pixels.
    **tileSize** : size |ro|, Tile size for tiles in this tileset in pixels (has ``width`` and ``height`` members).
    **tileSpacing** : int |ro|, Spacing between tiles in this tileset in pixels.
    **margin** : int |ro|, Margin around the tileset in pixels (only used at the top and left sides of the tileset image).
    **tileOffset** : :ref:`script-point`, Offset in pixels that is applied when tiles from this tileset are rendered.
    **backgroundColor** : color, Background color for this tileset in the *Tilesets* view.
    **selectedTiles** : [:ref:`script-tile`], Selected tiles (in the tileset editor).

Functions
~~~~~~~~~

new Tileset([name : string])
    Constructs a new tileset.

Tileset.tile(id : int) : :ref:`script-tile`
    Returns a reference to the tile with the given ID. Raises an error if no
    such tile exists. When the tile gets removed from the tileset, the
    reference changes to a standalone copy of the tile.

    Note that the tiles in a tileset are only guaranteed to have consecutive
    IDs for tileset-image based tilesets. For image collection tilesets there
    will be gaps when tiles have been removed from the tileset.

.. _script-tile:

Tile
^^^^

Inherits :ref:`script-object`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **id** : int |ro|, ID of this tile within its tileset.
    **width** : int |ro|, Width of the tile in pixels.
    **height** : int |ro|, Height of the tile in pixels.
    **size** : size |ro|, Size of the tile in pixels (has ``width`` and ``height`` members).
    **type** : string, Type of the tile.
    **imageFileName** : string, File name of the tile image (when the tile is part of an image collection tileset).
    **terrain** : :ref:`script-tileterrains`, An object specifying the terrain at each corner of the tile.
    **probability** : number, Probability that the tile gets chosen relative to other tiles.
    **objectGroup** : :ref:`script-objectgroup`, The :ref:`script-objectgroup` associated with the tile in case collision shapes were defined. Returns ``null`` if no collision shapes were defined for this tile.
    **frames** : :ref:`[frame] <script-frames>`, This tile's animation as an array of frames.
    **animated** : bool |ro|, Indicates whether this tile is animated.
    **tileset** : :ref:`script-tileset` |ro|, The tileset of the tile.

.. _script-tile-flags:

.. csv-table::
    :header: "Tile.Flags"

    Tile.FlippedHorizontally
    Tile.FlippedVertically
    Tile.FlippedAntiDiagonally
    Tile.RotatedHexagonal120

.. _script-tile-corner:

.. csv-table::
    :header: "Tile.Corner"

    Tile.TopLeft
    Tile.TopRight
    Tile.BottomLeft
    Tile.BottomRight

Functions
~~~~~~~~~

Tile.terrainAtCorner(corner : :ref:`Corner <script-tile-corner>`) : :ref:`script-terrain`
    Returns the terrain used at the given corner.

Tile.setTerrainAtCorner(corner : :ref:`Corner <script-tile-corner>`, :ref:`script-terrain`) : void
    Sets the terrain used at the given corner.

.. _script-terrain:

Terrain
^^^^^^^

Inherits :ref:`script-object`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **id** : int |ro|, ID of this terrain.
    **name** : string, Name of the terrain.
    **imageTile** : :ref:`script-tile`, The tile representing the terrain (needs to be from the same tileset).
    **tileset** : :ref:`script-tileset` |ro|, The tileset of the terrain.

.. _script-tilelayeredit:

TileLayerEdit
^^^^^^^^^^^^^

This object enables modifying the tiles on a tile layer. Tile layers can't be
modified directly for reasons of efficiency. The :ref:`apply() <script-tilelayeredit-apply>`
function needs to be called when you're done making changes.

An instance of this object is created by calling :ref:`TileLayer.edit() <script-tilelayer-edit>`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **target** : :ref:`script-tilelayer` |ro|, The target layer of this edit object.
    **mergeable** : bool, "Whether applied edits are mergeable with previous edits. Starts out as ``false`` and is automatically set to ``true`` by :ref:`apply() <script-tilelayeredit-apply>`."

Functions
~~~~~~~~~

TileLayerEdit.setTile(x : int, y : int, tile : :ref:`script-tile` [, flags : int = 0]) : void
    Sets the tile at the given location, optionally specifying :ref:`tile flags <script-tile-flags>`.

.. _script-tilelayeredit-apply:

TileLayerEdit.apply() : void
    Applies all changes made through this object. This object can be reused to
    make further changes.

.. _script-selectedarea:

SelectedArea
^^^^^^^^^^^^

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **boundingRect** : :ref:`script-rect` |ro|, Bounding rectangle of the selected area.

Functions
~~~~~~~~~

SelectedArea.get() : :ref:`script-region`
    Returns the selected region.

SelectedArea.set(rect : :ref:`script-rect`) : void
    Sets the selected area to the given rectangle.

SelectedArea.set(region : :ref:`script-region`) : void
    Sets the selected area to the given region.

SelectedArea.add(rect : :ref:`script-rect`) : void
    Adds the given rectangle to the selected area.

SelectedArea.add(region : :ref:`script-region`) : void
    Adds the given region to the selected area.

SelectedArea.subtract(rect : :ref:`script-rect`) : void
    Subtracts the given rectangle from the selected area.

SelectedArea.subtract(region : :ref:`script-region`) : void
    Subtracts the given region from the selected area.

SelectedArea.intersect(rect : :ref:`script-rect`) : void
    Sets the selected area to the intersection of the current selected area and the given rectangle.

SelectedArea.intersect(region : :ref:`script-region`) : void
    Sets the selected area to the intersection of the current selected area and the given region.


.. |ro| replace:: *[read‑only]*

.. _script-action:

Action
^^^^^^

An action that was registered with :ref:`tiled.registerAction() <script-registerAction>`.
This class is used to change the properties of the action. It can be added to a menu using
:ref:`tiled.extendMenu() <script-extendMenu>`.

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **checkable** : bool, Whether the action can be checked.
    **checked** : bool, Whether the action is checked.
    **enabled** : bool, Whether the action is enabled.
    **icon** : string, File name of an icon.
    **iconVisibleInMenu** : bool, Whether the action should show an icon in a menu.
    **id** : string |ro|, The ID this action was registered with.
    **shortcut** : QKeySequence, The shortcut (can be assigned a string like "Ctrl+K").
    **text** : string, The text used when the action is part of a menu.
    **visible** : bool, Whether the action is visible.

Functions
~~~~~~~~~

Action.trigger() : void
    Triggers the action.

Action.toggle() : void
    Changes the checked state to its opposite state.


.. _script-tileseteditor:

Tileset Editor
^^^^^^^^^^^^^^

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **collisionEditor** : :ref:`script-tilecollisioneditor`, "Access the collision editor within the tileset editor."

.. _script-tilecollisioneditor:

Tile Collision Editor
^^^^^^^^^^^^^^^^^^^^^

Properties
~~~~~~~~~~

.. csv-table::
    :widths: 1, 2

    **selectedObjects** : [:ref:`script-mapobject`], Selected objects.

Functions
~~~~~~~~~

TileCollisionEditor.focusObject(object : :ref:`script-mapobject`) : void
    Focuses the given object in the collision editor view and makes sure its
    visible in its objects list. Does not automatically select the object.

.. _script-basic-types:

Basic Types
^^^^^^^^^^^

Some types are provided by the Qt Scripting Engine and others are added based
on the needs of the data types above. In the following the most important
ones are documented.

.. _script-alignment:

Alignment
~~~~~~~~~

.. csv-table::
    :header: "Qt.Alignment"
    :widths: 1, 2

    Qt.AlignLeft, 0x0001
    Qt.AlignRight, 0x0002
    Qt.AlignHCenter, 0x0004
    Qt.AlignJustify, 0x0008
    Qt.AlignTop, 0x0020
    Qt.AlignBottom, 0x0040
    Qt.AlignVCenter, 0x0080
    Qt.AlignCenter, Qt.AlignVCenter | Qt.AlignHCenter

.. _script-font:

Font
~~~~

.. csv-table::
    :widths: 1, 2

    **family** : string, The font family.
    **pixelSize** : int, Font size in pixels.
    **bold** : bool, Whether the font is bold.
    **italic** : bool, Whether the font is italic.
    **underline** : bool, Whether the text is underlined.
    **strikeOut** : bool, Whether the text is striked through.
    **kerning** : bool, Whether to use kerning when rendering the text.

.. _script-frames:

Frames
~~~~~~

An array of frames, which are objects with the following properties:

.. csv-table::
    :widths: 1, 2

    **tileId** : int, The local tile ID used to represent the frame.
    **duration** : int, Duration of the frame in milliseconds.

.. _script-rect:

rect
~~~~

``Qt.rect(x, y, width, height)`` can be used to create a rectangle.

**Properties**:

.. csv-table::
    :widths: 1, 2

    **x** : int, X coordinate of the rectangle.
    **y** : int, Y coordinate of the rectangle.
    **width** : int, Width of the rectangle.
    **height** : int, Height of the rectangle.

.. _script-region:

region
~~~~~~

**Properties**:

.. csv-table::
    :widths: 1, 2

    **boundingRect** : :ref:`script-rect` |ro|, Bounding rectangle of the region.


.. _script-point:

point
~~~~~

``Qt.point(x, y)`` can be used to create a point object.

**Properties**:

.. csv-table::
    :widths: 1, 2

    **x** : number, X coordinate of the point.
    **y** : number, Y coordinate of the point.

.. _script-polygon:

Polygon
~~~~~~~

A polygon is not strictly a custom type. It is an array of objects that each
have an ``x`` and ``y`` property, representing the points of the polygon.

To modify the polygon of a :ref:`script-mapobject`, change or set up the
polygon array and then assign it to the object.

.. _script-tileterrains:

Terrains
~~~~~~~~

An object specifying the terrain for each corner of a tile:

.. csv-table::

    **topLeft** : :ref:`script-terrain`
    **topRight** : :ref:`script-terrain`
    **bottomLeft** : :ref:`script-terrain`
    **bottomRight** : :ref:`script-terrain`
