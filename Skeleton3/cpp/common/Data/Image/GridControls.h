/***
 * Entry point for clients wishing to change grid settings.
 *
 */

#pragma once

#include "State/ObjectManager.h"
#include "State/StateInterface.h"

namespace Carta {

namespace Data {

class DataGrid;

class GridControls : public QObject, public Carta::State::CartaObject{

    Q_OBJECT

public:
    /**
     * Set the grid axes color.
     * @param redAmount - an integer in [0, 255] indicating the amount of red.
     * @param greenAmount  an integer in [0,255] indicating the amount of green.
     * @param blueAmount - an integer in [0,255] indicating the amount of blue.
     * @return a list of errors or an empty list if the color was successfully set.
     */
    QStringList setAxesColor( int redAmount, int greenAmount, int blueAmount );

    /**
     * Set axis thickness.
     * @param thickness - a nonnegative value between 0 and 1.
     * @return an error message if the thickness could not be set or an empty string
     *  if the thickness was successfully set.
     */
    QString setAxesThickness( double thickness );

    /**
     * Set the axis transparency.
     * @param transparency - a nonnegative integer between 0 and 255, with 255 opaque.
     * @return an error message if the transparency could not be set or an empty string
     *      if it was successfully set.
     */
    QString setAxesTransparency( int transparency );

    /**
     * Set whether or not grid control settings should apply to all images on the set.
     * @param applyAll - true if the settings apply to all images on the stack;
     *      false otherwise.
     */
    void setApplyAll( bool applyAll );

    /**
     * Set the grid coordinate system.
     * @param coordSystem - an identifier for a grid coordinate system.
     * @return an error message if there was a problem setting the coordinate system;
     *  an empty string otherwise.
     */
    QString setCoordinateSystem( const QString& coordSystem );

    /**
     * Set the font family used for grid labels.
     * @param fontFamily - an identifier for a font family.
     * @return an error message if there was a problem setting the font family;
     *  an empty string otherwise.
     */
    QString setFontFamily( const QString& fontFamily );

    /**
     * Set the font size used for grid labels.
     * @param fontSize - an identifier for a font point size.
     * @return an error message if there was a problem setting the font point size;
     *  an empty string otherwise.
     */
    QString setFontSize( int fontSize );

    /**
     * Set the grid color.
     * @param redAmount - an integer in [0, 255] indicating the amount of red.
     * @param greenAmount  an integer in [0,255] indicating the amount of green.
     * @param blueAmount - an integer in [0,255] indicating the amount of blue.
     * @return a list of errors or an empty list if the color was successfully set.
     */
    QStringList setGridColor( int redAmount, int greenAmount, int blueAmount );

    /**
     * Set the spacing between grid lines.
     * @param spacing - the grid spacing in [0,1] with 1 having the least amount of spacing.
     * @return an error message if there was a problem setting the grid spacing; an empty
     *      string otherwise.
     */
    QString setGridSpacing( double spacing );

    /**
     * Set the thickness of the grid lines.
     * @param thickness - the grid line thickness in [0,1] with 1 having maximum thickness.
     * @return an error message if there was a problem setting the grid line thickness; an empty
     *      string otherwise.
     */
    QString setGridThickness( double thickness );

    /**
     * Set the transparency of the grid.
     * @param transparency - the amount of transparency in [0,255] with 255 completely opaque.
     * @return an error message if there was a problem setting the transparency; an empty
     *      string otherwise.
     */
    QString setGridTransparency( int transparency );

    /**
     * Set the color of grid labels color.
     * @param redAmount - an integer in [0, 255] indicating the amount of red.
     * @param greenAmount  an integer in [0,255] indicating the amount of green.
     * @param blueAmount - an integer in [0,255] indicating the amount of blue.
     * @return a list of errors or an empty list if the color was successfully set.
     */
    QStringList setLabelColor( int redAmount, int greenAmount, int blueAmount );

    /**
     * Set whether or not the axes should be shown.
     * @param showAxis - true if the axes should be shown; false otherwise.
     * @return an error message if there was a problem changing the visibility of the
     *      axes; an empty string otherwise.
     */
    QString setShowAxis( bool showAxis );

    /**
     * Set whether or not the coordinate system should be visible.
     * @param showCoordinateSystem - true if the coordinate system should be shown;
     *      false otherwise.
     * @return an error message if there was a problem setting the coordinate system;
     *      an empty string otherwise.
     */
    QString setShowCoordinateSystem( bool showCoordinateSystem );

    /**
     * Set whether or not the grid lines should be shown.
     * @param showLines - true if the grid lines should be shown; false otherwise.
     * @return an error message if there was a problem changing the visibility of the
     *     grid; an empty string otherwise.
     */
    QString setShowGridLines( bool showLines );

    /**
     * Set whether or not the axis should be internal or external.
     * @param showInternalLabels - true if the axes should be internal; false otherwise.
     * @return an error message if there was a problem setting the axes internal/external;
     *      false otherwise.
     */
    QString setShowInternalLabels( bool showInternalLabels );

    /**
     * Set whether or not to show axis ticks.
     * @param showTicks - true if the axis ticks should be shown; false otherwise.
     * @return an error message if there was a problem setting the visibility of axis
     *      ticks; and empty string otherwise.
     */
    QString setShowTicks( bool showTicks );

    /**
     * Set the color of the tick marks.
     * @param redAmount - a nonnegative integer in [0,255].
     * @param greenAmount - a nonnegative integer in [0,255].
     * @param blueAmount - a nonnegative integer in [0,255].
     * @return a list of error message(s) if there was a problem setting the tick
     *      color; an empty list otherwise.
     */
    QStringList setTickColor( int redAmount, int greenAmount, int blueAmount );

    /**
     * Set the length of the ticks.
     * @param tickThickness - a number between 0 and 1.
     * @return an error message if the tick thickness was not successfully set;
     *      an empty string otherwise.
     */
    QString setTickThickness( double tickThickness );

    /**
     * Set the transparency of the tick marks.
     * @param transparency - a nonnegative integer between 0 and 255 with 255 being
     *      opaque.
     * @return an error message if the transparency was not successfully set; an
     *      empty string otherwise.
     */
    QString setTickTransparency( int transparency );

    /**
     * Set the canvas theme.
     * @param theme - an identifier for a canvas theme.
     * @return an error message if the theme was not successfully set; an empty
     *      string otherwise.
     */
    QString setTheme( const QString& theme );

    virtual ~GridControls();
    const static QString CLASS_NAME;

signals:
    void gridChanged( const Carta::State::StateInterface& gridState );

private:

    void _initializeDefaultState();
    void _initializeDefaultState( const QString& key );
    void _initializeCallbacks();
    QStringList _parseColorParams( const QString& params, const QString& label,
            int* red, int* green, int* blue ) const;
    void _updateGrid();

    const static QString ALL;
    static bool m_registered;

    GridControls( const QString& path, const QString& id );

    class Factory;

    std::unique_ptr<DataGrid> m_dataGrid;

	GridControls( const GridControls& other);
	GridControls& operator=( const GridControls& other );
};
}
}
