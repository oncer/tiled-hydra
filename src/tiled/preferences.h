/*
 * preferences.h
 * Copyright 2009-2011, Thorbjørn Lindeijer <thorbjorn@lindeijer.nl>
 * Copyright 2016, Mamed Ibrahimov <ibramlab@gmail.com>
 *
 * This file is part of Tiled.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <QColor>
#include <QDate>
#include <QObject>

#include "map.h"
#include "objecttypes.h"

class QSettings;

namespace Tiled {

/**
 * This class holds user preferences and provides a convenient interface to
 * access them.
 */
class Preferences : public QObject
{
    Q_OBJECT

public:
    static Preferences *instance();
    static void deleteInstance();

    bool showGrid() const;
    bool showTileObjectOutlines() const;
    bool showTileAnimations() const;
    bool showTileCollisionShapes() const;
    bool snapToGrid() const;
    bool snapToFineGrid() const;
    bool snapToPixels() const;
    QColor gridColor() const;
    int gridFine() const;
    qreal objectLineWidth() const;

    bool highlightCurrentLayer() const;
    bool highlightHoveredObject() const;
    bool showTilesetGrid() const;

    enum ObjectLabelVisiblity {
        NoObjectLabels,
        SelectedObjectLabels,
        AllObjectLabels
    };

    ObjectLabelVisiblity objectLabelVisibility() const;
    void setObjectLabelVisibility(ObjectLabelVisiblity visiblity);

    bool labelForHoveredObject() const;
    void setLabelForHoveredObject(bool enabled);

    enum ApplicationStyle {
        SystemDefaultStyle,
        FusionStyle,
        TiledStyle
    };

    ApplicationStyle applicationStyle() const;
    void setApplicationStyle(ApplicationStyle style);

    QColor baseColor() const;
    void setBaseColor(const QColor &color);

    QColor selectionColor() const;
    void setSelectionColor(const QColor &color);

    Map::LayerDataFormat layerDataFormat() const;
    void setLayerDataFormat(Map::LayerDataFormat layerDataFormat);

    Map::RenderOrder mapRenderOrder() const;
    void setMapRenderOrder(Map::RenderOrder mapRenderOrder);

    bool dtdEnabled() const;
    void setDtdEnabled(bool enabled);

    bool safeSavingEnabled() const;
    void setSafeSavingEnabled(bool enabled);

    enum ExportOption {
        EmbedTilesets                   = 0x1,
        DetachTemplateInstances         = 0x2,
        ResolveObjectTypesAndProperties = 0x4,
        ExportMinimized                 = 0x8,
    };
    Q_DECLARE_FLAGS(ExportOptions, ExportOption)

    ExportOptions exportOptions() const;
    void setExportOption(ExportOption option, bool value);
    bool exportOption(ExportOption option) const;

    QString language() const;
    void setLanguage(const QString &language);

    bool reloadTilesetsOnChange() const;
    void setReloadTilesetsOnChanged(bool value);

    bool useOpenGL() const;
    void setUseOpenGL(bool useOpenGL);

    void setObjectTypes(const ObjectTypes &objectTypes);

    enum FileType {
        ObjectTypesFile,
        ObjectTemplateFile,
        ImageFile,
        ExportedFile,
        ExternalTileset,
        WorldFile
    };

    QString lastPath(FileType fileType) const;
    void setLastPath(FileType fileType, const QString &path);

    bool automappingDrawing() const;

    QString mapsDirectory() const;
    void setMapsDirectory(const QString &path);

    QString stampsDirectory() const;
    void setStampsDirectory(const QString &stampsDirectory);

    QString templatesDirectory() const;
    void setTemplatesDirectory(const QString &path);

    QString objectTypesFile() const;
    void setObjectTypesFile(const QString &filePath);

    QDate firstRun() const;
    int runCount() const;

    bool isPatron() const;
    void setPatron(bool isPatron);

    bool shouldShowPatreonDialog() const;
    void setPatreonDialogReminder(const QDate &date);

    enum { MaxRecentFiles = 8 };
    QStringList recentFiles() const;
    QString fileDialogStartLocation() const;
    void addRecentFile(const QString &fileName);

    bool openLastFilesOnStartup() const;

    bool checkForUpdates() const;
    void setCheckForUpdates(bool on);

    bool displayNews() const;
    void setDisplayNews(bool on);

    bool wheelZoomsByDefault() const;

    /**
     * Provides access to the QSettings instance to allow storing/retrieving
     * arbitrary values. The naming style for groups and keys is CamelCase.
     */
    QSettings *settings() const;

public slots:
    void setShowGrid(bool showGrid);
    void setShowTileObjectOutlines(bool enabled);
    void setShowTileAnimations(bool enabled);
    void setShowTileCollisionShapes(bool enabled);
    void setSnapToGrid(bool snapToGrid);
    void setSnapToFineGrid(bool snapToFineGrid);
    void setSnapToPixels(bool snapToPixels);
    void setGridColor(QColor gridColor);
    void setGridFine(int gridFine);
    void setObjectLineWidth(qreal lineWidth);
    void setHighlightCurrentLayer(bool highlight);
    void setHighlightHoveredObject(bool highlight);
    void setShowTilesetGrid(bool showTilesetGrid);
    void setAutomappingDrawing(bool enabled);
    void setOpenLastFilesOnStartup(bool load);
    void setPluginEnabled(const QString &fileName, bool enabled);
    void setWheelZoomsByDefault(bool mode);

    void clearRecentFiles();

signals:
    void showGridChanged(bool showGrid);
    void showTileObjectOutlinesChanged(bool enabled);
    void showTileAnimationsChanged(bool enabled);
    void showTileCollisionShapesChanged(bool enabled);
    void snapToGridChanged(bool snapToGrid);
    void snapToFineGridChanged(bool snapToFineGrid);
    void snapToPixelsChanged(bool snapToPixels);
    void gridColorChanged(QColor gridColor);
    void gridFineChanged(int gridFine);
    void objectLineWidthChanged(qreal lineWidth);
    void highlightCurrentLayerChanged(bool highlight);
    void highlightHoveredObjectChanged(bool highlight);
    void showTilesetGridChanged(bool showTilesetGrid);
    void objectLabelVisibilityChanged(ObjectLabelVisiblity);
    void labelForHoveredObjectChanged(bool enabled);

    void applicationStyleChanged(ApplicationStyle);
    void baseColorChanged(const QColor &baseColor);
    void selectionColorChanged(const QColor &selectionColor);

    void useOpenGLChanged(bool useOpenGL);

    void languageChanged();

    void objectTypesChanged();

    void mapsDirectoryChanged();
    void stampsDirectoryChanged(const QString &stampsDirectory);
    void templatesDirectoryChanged(const QString &templatesDirectory);

    void isPatronChanged();

    void recentFilesChanged();

    void checkForUpdatesChanged(bool on);
    void displayNewsChanged(bool on);

private:
    Preferences();
    ~Preferences() override;

    bool boolValue(const char *key, bool def = false) const;
    QColor colorValue(const char *key, const QColor &def = QColor()) const;
    QString stringValue(const char *key, const QString &def = QString()) const;
    int intValue(const char *key, int defaultValue) const;
    qreal realValue(const char *key, qreal defaultValue) const;

    QSettings *mSettings;

    bool mShowGrid;
    bool mShowTileObjectOutlines;
    bool mShowTileAnimations;
    bool mShowTileCollisionShapes;
    bool mSnapToGrid;
    bool mSnapToFineGrid;
    bool mSnapToPixels;
    QColor mGridColor;
    int mGridFine;
    qreal mObjectLineWidth;
    bool mHighlightCurrentLayer;
    bool mHighlightHoveredObject;
    bool mShowTilesetGrid;
    bool mOpenLastFilesOnStartup;
    ObjectLabelVisiblity mObjectLabelVisibility;
    bool mLabelForHoveredObject;
    ApplicationStyle mApplicationStyle;
    QColor mBaseColor;
    QColor mSelectionColor;

    Map::LayerDataFormat mLayerDataFormat;
    Map::RenderOrder mMapRenderOrder;
    bool mDtdEnabled;
    bool mSafeSavingEnabled;
    ExportOptions mExportOptions;
    QString mLanguage;
    bool mReloadTilesetsOnChange;
    bool mUseOpenGL;

    bool mAutoMapDrawing;

    QString mMapsDirectory;
    QString mStampsDirectory;
    QString mTemplatesDirectory;
    QString mObjectTypesFile;

    QDate mFirstRun;
    QDate mPatreonDialogTime;
    int mRunCount;
    bool mIsPatron;
    bool mCheckForUpdates;
    bool mDisplayNews;
    bool mWheelZoomsByDefault;

    static Preferences *mInstance;
};


inline bool Preferences::showGrid() const
{
    return mShowGrid;
}

inline bool Preferences::showTileObjectOutlines() const
{
    return mShowTileObjectOutlines;
}

inline bool Preferences::showTileAnimations() const
{
    return mShowTileAnimations;
}

inline bool Preferences::showTileCollisionShapes() const
{
    return mShowTileCollisionShapes;
}

inline bool Preferences::snapToGrid() const
{
    return mSnapToGrid;
}

inline bool Preferences::snapToFineGrid() const
{
    return mSnapToFineGrid;
}

inline bool Preferences::snapToPixels() const
{
    return mSnapToPixels;
}

inline QColor Preferences::gridColor() const
{
    return mGridColor;
}

inline int Preferences::gridFine() const
{
    return mGridFine;
}

inline qreal Preferences::objectLineWidth() const
{
    return mObjectLineWidth;
}

inline bool Preferences::highlightCurrentLayer() const
{
    return mHighlightCurrentLayer;
}

inline bool Preferences::highlightHoveredObject() const
{
    return mHighlightHoveredObject;
}

inline bool Preferences::showTilesetGrid() const
{
    return mShowTilesetGrid;
}

inline Preferences::ObjectLabelVisiblity Preferences::objectLabelVisibility() const
{
    return mObjectLabelVisibility;
}

inline bool Preferences::labelForHoveredObject() const
{
    return mLabelForHoveredObject;
}

inline Preferences::ApplicationStyle Preferences::applicationStyle() const
{
    return mApplicationStyle;
}

inline QColor Preferences::baseColor() const
{
    return mBaseColor;
}

inline QColor Preferences::selectionColor() const
{
    return mSelectionColor;
}

inline Map::LayerDataFormat Preferences::layerDataFormat() const
{
    return mLayerDataFormat;
}

inline Map::RenderOrder Preferences::mapRenderOrder() const
{
    return mMapRenderOrder;
}

inline bool Preferences::dtdEnabled() const
{
    return mDtdEnabled;
}

inline bool Preferences::safeSavingEnabled() const
{
    return mSafeSavingEnabled;
}

inline Preferences::ExportOptions Preferences::exportOptions() const
{
    return mExportOptions;
}

inline bool Preferences::exportOption(ExportOption option) const
{
    return mExportOptions.testFlag(option);
}

inline QString Preferences::language() const
{
    return mLanguage;
}

inline bool Preferences::reloadTilesetsOnChange() const
{
    return mReloadTilesetsOnChange;
}

inline bool Preferences::useOpenGL() const
{
    return mUseOpenGL;
}

inline bool Preferences::automappingDrawing() const
{
    return mAutoMapDrawing;
}

inline QString Preferences::mapsDirectory() const
{
    return mMapsDirectory;
}

inline QDate Preferences::firstRun() const
{
    return mFirstRun;
}

inline int Preferences::runCount() const
{
    return mRunCount;
}

inline bool Preferences::isPatron() const
{
    return mIsPatron;
}

inline bool Preferences::checkForUpdates() const
{
    return mCheckForUpdates;
}

inline bool Preferences::displayNews() const
{
    return mDisplayNews;
}

inline bool Preferences::openLastFilesOnStartup() const
{
    return mOpenLastFilesOnStartup;
}

inline bool Preferences::wheelZoomsByDefault() const
{
    return mWheelZoomsByDefault;
}

inline QSettings *Preferences::settings() const
{
    return mSettings;
}

} // namespace Tiled

Q_DECLARE_OPERATORS_FOR_FLAGS(Tiled::Preferences::ExportOptions)
