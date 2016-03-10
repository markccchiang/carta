/***
 * Meshes together data, selection(s), and view(s).
 */

#pragma once

#include <State/StateInterface.h>
#include <State/ObjectManager.h>
#include <Data/Image/IPercentIntensityMap.h>
#include "CartaLib/CartaLib.h"
#include "CartaLib/AxisInfo.h"
#include "CartaLib/RegionInfo.h"

#include <QString>
#include <QList>
#include <QObject>
#include <QImage>

#include <set>

class CoordinateFormatterInterface;

namespace Carta {
    namespace Lib {
        namespace PixelPipeline {
            class CustomizablePixelPipeline;
        }
        namespace Image {
            class ImageInterface;
        }
        namespace NdArray {
            class RawViewInterface;
        }
    }
}

namespace Carta {
namespace Data {
class ColorState;
class Layer;
class Stack;
class DataSource;
class DisplayControls;
class GridControls;
class ContourControls;
class Settings;
class Region;
class RegionRectangle;

class Controller: public QObject, public Carta::State::CartaObject,
    public IPercentIntensityMap {

    friend class Animator;
    friend class Colormap;
    friend class Profiler;

    Q_OBJECT

public:
    
    /**
     * Clear the view.
     */
    void clear();

    /**
     * Add a contour set to the selected images.
     * @param contourSet - the contour set to add.
     */
    virtual void addContourSet( std::shared_ptr<DataContours> contourSet) Q_DECL_OVERRIDE;

    /**
     * Add data to this controller.
     * @param fileName the location of the data;
     *        this could represent a url or an absolute path on a local filesystem.
     * @return true upon success, false otherwise.
     */
    bool addData(const QString& fileName);

    /**
     * Apply the indicated clips to managed images.
     * @param minIntensityPercentile the minimum clip percentile [0,1].
     * @param maxIntensityPercentile the maximum clip percentile [0,1].
     * @return a QString indicating if there was an error applying the clips or an empty
     *      string if there was not an error.
     */
    QString applyClips( double minIntensityPercentile, double maxIntensityPercentile );

    /**
     * Center the image on the pixel with coordinates (x, y).
     * @param imgX the x-coordinate for the center of the pan.
     * @param imgY the y-coordinate for the center of the pan.
     */
    void centerOnPixel( double imgX , double imgY);

    /**
     * Close the given image.
     * @param name an identifier for the image to close.
     * @return - an error message if the image was not successfully closed.
     */
    QString closeImage( const QString& name );

    /**
     * Close the given region.
     * @param regionId - an identifier for a region.
     * @return - an error message if the region was not successfully closed.
     */
    QString closeRegion( const QString& regionId );

    /**
      * Get the image pixel that is currently centered.
      * @return a QPointF value consisting of the x- and y-coordinates of
      * the center pixel, or a special value of (-0.0, -0.0) if the
      * center pixel could not be obtained.
      */
    QPointF getCenterPixel();

    /**
     * Return the coordinate system in use.
     * @return - an enumerated coordinate system type.
     */
    Carta::Lib::KnownSkyCS getCoordinateSystem() const;

    /**
     * Return the minimum clip percentile.
     * @return the minimum clip percentile.
     */
    double getClipPercentileMin() const;

    /**
     * Return the maximum clip percentile.
     * @return the maximum clip percentile.
     */
    double getClipPercentileMax() const;

    /**
     * Return the coordinates at pixel (x, y) in the given coordinate system.
     * @param x the x-coordinate of the desired pixel.
     * @param y the y-coordinate of the desired pixel.
     * @param system the desired coordinate system.
     * @return the coordinates at pixel (x, y).
     */
    QStringList getCoordinates( double x, double y, Carta::Lib::KnownSkyCS system ) const;


    /**
     * Return a list of images that have been loaded.
     * @return - a list of loaded images.
     */
    std::vector<std::shared_ptr<Carta::Lib::Image::ImageInterface> > getImages();

    /**
     * Return the data source of the selected image.
     * @return - the data source of the selected image.
     */
    std::shared_ptr<DataSource> getDataSource();

    /**
     * Return all data sources.
     * @return - the list of all visible data sources.
     */
    std::vector< std::shared_ptr<DataSource> > getDataSources();

    /**
     * Return a shared pointer to the contour controls.
     * @return - a shared pointer to the contour controls.
     */
    std::shared_ptr<ContourControls> getContourControls();

    /**
     * Return the current frame for the the axis of the indicated type.
     * @param axisType - an identifier for the type of axis.
     * @return the current index withen the axis.
     */
    int getFrame( Carta::Lib::AxisInfo::KnownType axisType ) const;

    /**
     * Return the frame upper bound.
     * @param type - the axis for which a frame upper bound is needed.
     * @return the largest frame for a particular axis in the image.
     */
    int getFrameUpperBound( Carta::Lib::AxisInfo::KnownType type ) const;

    /**
     * Return a shared pointer to the grid controls.
     * @return - a shared pointer to the grid controls.
     */
    std::shared_ptr<GridControls> getGridControls();

    /**
     * Get the image dimensions.
     */
    //std::vector<int> getImageDimensions( ) const;

    /**
     * Returns an identifier for the layer at the given index.
     * @param index the index of a data layer.
     * @return an identifier for the layer.
     */
    //QString getLayerId( int index ) const;

    /**
     * Return a list of indices indicating the current frames of the selected
     * image.
     * @return - a list consisting of the current frames of the current image.
     */
    std::vector<int> getImageSlice() const;

    /**
     * Returns the intensity corresponding to a given percentile in the current frame.
     * @param percentile a number [0,1] for which an intensity is desired.
     * @param intensity the computed intensity corresponding to the percentile.
     * @return true if the computed intensity is valid; otherwise false.
     */
    bool getIntensity( double percentile, double* intensity ) const;

    /**
     * Returns the intensity corresponding to a given percentile.
     * @param frameLow a lower bound for the image channels or -1 if there is no lower bound.
     * @param frameHigh an upper bound for the image channels or -1 if there is no upper bound.
     * @param percentile a number [0,1] for which an intensity is desired.
     * @param intensity the computed intensity corresponding to the percentile.
     * @return true if the computed intensity is valid; otherwise false.
     */
    bool getIntensity( int frameLow, int frameHigh, double percentile, double* intensity ) const;

    /**
     * Get the dimensions of the image viewer (window size).
     */
    QSize getOutputSize( );

    /**
     * Return the percentile corresponding to the given intensity in the current frame.
     * @param intensity a value for which a percentile is needed.
     * @return the percentile corresponding to the intensity.
     */
    double getPercentile( double intensity ) const;

    /**
     * Return the percentile corresponding to the given intensity.
     * @param frameLow a lower bound for the channel range or -1 if there is no lower bound.
     * @param frameHigh an upper bound for the channel range or -1 if there is no upper bound.
     * @param intensity a value for which a percentile is needed.
     * @return the percentile corresponding to the intensity.
     */
    double getPercentile( int frameLow, int frameHigh, double intensity ) const;

    /**
     * Return the pixel coordinates corresponding to the given world coordinates.
     * @param ra the right ascension (in radians) of the world coordinates.
     * @param dec the declination (in radians) of the world coordinates.
     * @return a list consisting of the x- and y-coordinates of the pixel
     *  corresponding to the given world coordinates.
     */
    QStringList getPixelCoordinates( double ra, double dec ) const;

    /**
     * Return the value of the pixel at (x, y).
     * @param x the x-coordinate of the desired pixel.
     * @param y the y-coordinate of the desired pixel.
     * @return the value of the pixel at (x, y), or blank if it could not be obtained.
     */
    QString getPixelValue( double x, double y ) const;

    /**
     * Return the units of the pixels.
     * @return the units of the pixels, or blank if units could not be obtained.
     */
    QString getPixelUnits() const;

    /**
     * Return a list of information about loaded regions.
     * @return - a list of region information.
     */
    std::vector<Carta::Lib::RegionInfo> getRegions() const;

    /**
     * Return the index of the image that is currently at the top of the stack.
     * @return the index of the current image.
     */
    int getSelectImageIndex() const ;

    /**
     * Get the color map information for the data sources that have been
     * selected.
     * @return - a list containing color map information for the data sources
     *      that have been selected.
     */
    std::vector< std::shared_ptr<ColorState> > getSelectedColorStates();

    /**
     * Return a count of the number of image layers in the stack.
     * @return the number of image layers in the stack.
     */
    //Note:  this will include image layers that the user may not see because they
    //are hidden.
    int getStackedImageCount() const;

    /**
     * Returns the number of visibile image layers in the stack.
     * @return a count of the number of image layers that have not been hidden
     *      and are available for the user to see.
     */
    int getStackedImageCountVisible() const;

    /**
      * Returns a json string representing the state of this controller.
      * @param type - the type of snapshot to return.
      * @param sessionId - an identifier for the user's session.
      * @return a string representing the state of this controller.
      */
    virtual QString getStateString( const QString& sessionId, SnapshotType type ) const Q_DECL_OVERRIDE;

    /**
     * Get the current zoom level
     */
    double getZoomLevel( );

    /**
     * Returns whether or not the image stack layers are selected based on the
     * animator (auto) or whether the user has indicated a manual selection.
     * @return - true if the stack layers are selected based on the current layer; false
     *      if the user has specified manual selection.
     */
    bool isStackSelectAuto() const;

    /**
     * Remove a contour set from the images.
     * @param contourSet - the contour set to remove.
     */
     virtual void removeContourSet( std::shared_ptr<DataContours> contourSet ) Q_DECL_OVERRIDE;

    /**
     * Center the image.
     */
    void resetPan();

    /**
     * Restore the state from a string representation.
     * @param state- a json representation of state.
     */
    void resetState( const QString& state );

    /**
     * Reset the images that are loaded and other data associated state.
     * @param state - the data state.
     */
    virtual void resetStateData( const QString& state ) Q_DECL_OVERRIDE;

    /**
     * Reset the zoom to its original value.
     */
    void resetZoom();

    /**
     * Save a copy of the full image in the current image view.
     * @param filename the full path where the file is to be saved.
     * @param scale the scale (zoom level) of the saved image.
     * @return an error message if there is an initial problem with saving;
     *      an empty string if the save operation has been initiated.
     */
    QString saveImage( const QString& filename,  double scale );

    /**
     * Save a copy of the full image in the current image view using the current scale.
     * @param filename the full path where the file is to be saved.
     * @return an error message if there is an initial problem with saving;
     *      an empty string if the save operation has been initiated.
     */
    QString saveImage( const QString& filename );

    /**
     * Save the state of this controller.
     * @param flush - true if the state should be flushed to the client; false otherwise
     *      (for example, when the state change came from the client).
     */
    //void saveState( bool flush = true );

    /**
     * Set whether or not clip values should be recomputed when the frame changes.
     * @param autoClip - whether or not clips should be recomputed when the frame
     *      changed.
     */
    void setAutoClip( bool autoClip );

    /**
     *  Make a data selection.
     *  @param imageIndex - a String representing the index of a specific data selection.
     */
    void setFrameImage(int imageIndex);

    /**
     * Set the zoom level
     * @param zoomLevel either positive or negative depending on the desired zoom direction.
     */
    void setZoomLevel( double zoomLevel );

    /**
     * Set the overall clip amount for the data.
     * @param clipValue a number between 0 and 1.
     * @return an error message if the clip value cannot be set; otherwise and empty string.
     */
    QString setClipValue( double clipValue );

    /**
     * Set whether or not to apply a composition mode to the image.
     * @param compMode - the type of composition mode to apply.
     * @return an error message if there was a problem recognizing the composition mode.
     */
    QString setCompositionMode( const QString& id, const QString& compMode );


    /**
     * Specify a new image order.
     * @param groupId - an identifier for the group where the images will be reordered.
     * @param imageIndices - a list specifying a new order for the images in
     *      a layer.
     * @return an error message if the new image order could not be set;
     *      otherwise, an empty string.
     */
    QString setImageOrder( const QString& groupId, const std::vector<int>& imageIndices );

    /**
     * Show/hide a particular layer in the stack.
     * @param id - the identifier for a layer in the stack.
     * @param visible - true if the layer should be visible; false otherwise.
     */
    QString setImageVisibility( /*int dataIndex*/const QString& id, bool visible );

    /**
     * Set the indices of the selected data sources.
     * @param layers - a list of identifiers for layers that should be selected.
     */
    QString setLayersSelected( const QStringList layerNames );

    /**
     * Set the color to use for the mask.
     * @param redAmount - the amount of red in [0,255].
     * @param greenAmount - the amount of green in [0,255].
     * @param blueAmount - the amount of blue in [0,255].
     * @return - a list containing any errors that may have occurred in setting
     *      the mask color.
     */
    //Note: Mask color will not take affect unless a composition mode that supports
    //a color filter has been set.
    QStringList setMaskColor( const QString& id, int redAmount, int greenAmount, int blueAmount );

    /**
     * Set the transparency of the layer.
     * @param alphaAmount - the transparency level in [0,255] with 255 being opaque.
     * @return - an error message if there was a problem setting the layer opacity or
     *      an empty string otherwise.
     */
    //Note: Layer transparency will not take affect unless a composition mode which supports
    //transparency has been set.
    QString setMaskAlpha( const QString& id, int alphaAmount );

    /**
     * Set whether or not a pan/zoom operation should affect all layers in the stack
     * or just the top layer.
     * @param panZoomAll - true if all layers should be pan/zoomed; false if just the
     *      top layer should be pan/zoomed.
     */
    void setPanZoomAll( bool panZoomAll );

    /**
     * Group/ungroup the selected layers.
     * @param grouped - true if the selected layers should be grouped; false if they
     *  are grouped and should be ungrouped.
     * @return - an error message if the group/ungroup message could not be performed.
     */
    QString setSelectedLayersGrouped( bool grouped );

    /**
     * Set whether or not selection of layers in the stack should be based on the
     * current layer or whether the user wants to make a manual selection.
     * @param automatic - true for automatic selection; false for manual selection.
     */
    void setStackSelectAuto( bool automatic );

    /**
     * Store the index of the settings tab that was selected.
     * @param index - the index of the settings tab that was selected.
     * @return - an error message if the index could not be set; otherwise, an
     *  empty string.
     */
    QString setTabIndex( int index );


    /**
     * Change the pan of the current image.
     * @param imgX the x-coordinate for the center of the pan.
     * @param imgY the y-coordinate for the center of the pan.
     */
    void updatePan( double imgX , double imgY);


    /**
     * Update the zoom settings.
     * @param centerX the screen x-coordinate where the zoom was centered.
     * @param centerY the screen y-coordinate where the zoom was centered.
     * @param z either positive or negative depending on the desired zoom direction.
     */
    void updateZoom( double centerX, double centerY, double z );


    virtual ~Controller();

    static const QString CLASS_NAME;
    static const QString CURSOR;
    static const QString PLUGIN_NAME;

signals:

    /**
     * Notification that the horizontal/vertical display axes have changed.
     */
    void axesChanged();

    /**
      * Notification that the channel/selection managed by this controller has
      * changed.
      * @param controller this Controller.
      */
    void frameChanged( Controller* controller, Carta::Lib::AxisInfo::KnownType axis);

    /**
     * Notification that the image clip values have changed.
     * @param minPercentile - the new minimum clip percentile.
     * @param maxPercentile - the new maximum clip percentile.
     */
    void clipsChanged( double minPercentile, double maxPercentile );

    /**
     * Notification that one or more color map(s) have changed.
     */
    void colorChanged( Controller* controller );

    /**
     *  Notification that the image/selection managed by this controller has
     *  changed.
     *  @param controller this Controller.
     */
    void dataChanged(Controller* controller );

    /**
     *  Notification that the region/selection managed by this controller has
     *  changed.
     *  @param controller this Controller.
     */
    void dataChangedRegion( Controller* controller );


    /// Return the result of SaveFullImage() after the image has been rendered
    /// and a save attempt made.
    void saveImageResult( bool result );



protected:
    virtual QString getSnapType(CartaObject::SnapshotType snapType) const Q_DECL_OVERRIDE;

private slots:

    void _displayAxesChanged(std::vector<Carta::Lib::AxisInfo::KnownType> displayAxisTypes, bool applyAll);

    void _colorMapChanged();

    void _contourSetAdded( Layer* cData, const QString& setName );
    void _contourSetRemoved( const QString setName );
    void _contoursChanged();



    void _gridChanged( const Carta::State::StateInterface& state, bool applyAll );

    //Refresh the view based on the latest data selection information.
    //The parameter newClips is set if the clip values have changed and need to be recomputed.
    void _loadView( bool newClips = false );
    void _loadViewQueued( bool newClips );
    void _notifyFrameChange( Carta::Lib::AxisInfo::KnownType axis );


    // Asynchronous result from saveFullImage().
    void saveImageResultCB( bool result );

private:

    /**
     *  Constructor.
     */
    Controller( const QString& id, const QString& path );

    class Factory;

    /// Add a region to the stack from a file.
    bool _addDataRegion(const QString& fileName );

    /// Add an image to the stack from a file.
    bool _addDataImage( const QString& fileName );

    //bool _addGroup( const QString& groupState );

    //Clear the color map.
    void _clearColorMap();

    //Clear image statistics.
    void _clearStatistics();


    set<Carta::Lib::AxisInfo::KnownType> _getAxesHidden() const;
    std::vector<Carta::Lib::AxisInfo::KnownType> _getAxisZTypes() const;


    QString _getPreferencesId() const;

    //Provide default values for state.
    void _initializeState();
    void _initializeCallbacks();


    //QString _makeRegion( const QString& regionType );

    //Save region state.
    void _saveStateRegions();

    /**
     * Set whether or not the selected layers should be using the global
     * colormap.
     * @param global - true if selected layers should use the global color map;
     *      false, otherwise.
     */
    void _setColorMapUseGlobal( bool global );

    /**
     * Set the global color map..
     * @param colorState - the global color map information.
     */
    void _setColorMapGlobal( std::shared_ptr<ColorState> colorState );

    /**
     * Make a frame selection.
     * @param axisType - the axis for which a frame is being set.
     * @param frameIndex  a frame index for the axis.
     */
    void _setFrameAxis(int frameIndex, Carta::Lib::AxisInfo::KnownType axisType );
    QString _setLayersSelected( const QStringList indices, bool flush = true );


    void _updateCursor( int mouseX, int mouseY );
    void _updateCursorText(bool notifyClients );
    void _updateDisplayAxes( /*int targetIndex*/ );

    static bool m_registered;

    static const QString CLIP_VALUE_MIN;
    static const QString CLIP_VALUE_MAX;
    static const QString CLOSE_IMAGE;
    static const QString AUTO_CLIP;
    static const QString DATA;
    static const QString DATA_PATH;
    static const QString IMAGE;
    static const QString PAN_ZOOM_ALL;
    static const QString REGIONS;
    static const QString CENTER;
    static const QString POINTER_MOVE;
    static const QString STACK_SELECT_AUTO;

    static const QString ZOOM;

    std::shared_ptr<GridControls> m_gridControls;
    std::shared_ptr<ContourControls> m_contourControls;

    std::unique_ptr<Settings> m_settings;

    //Data available to and managed by this controller.
    std::unique_ptr<Stack> m_stack;

    std::shared_ptr<ColorState> m_stateColor;


    QList<std::shared_ptr<Region> > m_regions;

    //Holds image that are loaded and selections on the data.
    Carta::State::StateInterface m_stateData;

    //Separate state for mouse events since they get updated rapidly and not
    //everyone wants to listen to them.
    Carta::State::StateInterface m_stateMouse;

    Controller(const Controller& other);
    Controller& operator=(const Controller& other);

};

}
}
