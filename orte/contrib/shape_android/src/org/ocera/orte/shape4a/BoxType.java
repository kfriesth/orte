/**
 * 
 *	This file is part of shape4a.
 *
 *  shape4a is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  shape4a is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with shape4a.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
package org.ocera.orte.shape4a;

import org.ocera.orte.DomainApp;
import org.ocera.orte.types.MessageData;
import org.ocera.orte.types.ORTEConstant;

import android.util.Log;

/**
 * Class for object sent and received throw the ORTE middleware.
 * 
 * @author jiri hubacek <jiri.hubacek@gmail.com>
 * @version %I, %G
 * @since 1.1
 */
public class BoxType extends MessageData
{
	private static final byte FUZZY = 0;
	private static final double DESTINATION_WIDTH = 389.0;
	private static final double DESTINATION_HEIGHT = 331.0;
	
	public byte color;
	public byte shape;
	public BoxRect rectangle = new BoxRect();
	
	public boolean allowScaling;
	private double scaleWidth;
	private double scaleHeight;
	
	/**
	 * Register new domain type, set default params.
	 * 
	 * @since 1.1
	 */
	public BoxType(DomainApp appDomain, String newTopic)
	{
		super();
		this.setTopic(newTopic);
		
		if (!appDomain.regNewDataType("BoxType", this.getMaxDataLength())) {
			Log.e("BoxType", "Cannot register data type 'BoxType'.");
		}
		
		this.allowScaling = true;
	}

	/* (non-Javadoc)
	 * @see org.ocera.orte.types.MessageData#read()
	 */
	@Override
	public void read() 
	{
		buffer.rewind();
		
		// get color
		this.color = buffer.get();
		
		// skip fuzzy bytes
		buffer.get();buffer.get();buffer.get();
		
		// get shape
		this.shape = buffer.get();
		
		// skip fuzzy bytes
		buffer.get();buffer.get();buffer.get();
		
		// get rect position (with scaling)
		if (this.allowScaling) {
			this.rectangle.top_left_x = (short) (buffer.getShort() / this.scaleWidth);
			this.rectangle.top_left_y = (short) (buffer.getShort() / this.scaleHeight);
			this.rectangle.bottom_right_x = (short) (buffer.getShort() / this.scaleWidth);
			this.rectangle.bottom_right_y = (short) (buffer.getShort() / this.scaleHeight);
		} else {
			this.rectangle.top_left_x = buffer.getShort();
			this.rectangle.top_left_y = buffer.getShort();
			this.rectangle.bottom_right_x = buffer.getShort();
			this.rectangle.bottom_right_y = buffer.getShort();
		}
		
		// don't care about last fuzzy byte because of buffer.rewind()
		
		//Log.d("BoxType", "receiving - color: "+this.color+", shape: "+this.shape+ ", rectangle:{"+this.rectangle.top_left_x+","+this.rectangle.top_left_y+","+this.rectangle.bottom_right_x+","+this.rectangle.bottom_right_y+",}");
	}

	/* (non-Javadoc)
	 * @see org.ocera.orte.types.MessageData#write()
	 */
	@Override
	public void write()
	{
		buffer.rewind();
		
		// put color
		buffer.put(this.color);
		
		// put fuzzy bytes
		buffer.put(BoxType.FUZZY);buffer.put(BoxType.FUZZY);buffer.put(BoxType.FUZZY);
		
		// put shape
		buffer.put(this.shape);
		
		// put fuzzy bytes
		buffer.put(BoxType.FUZZY);buffer.put(BoxType.FUZZY);buffer.put(BoxType.FUZZY);
		
		// put rectange position (with scaling)
		if (this.allowScaling) {
			buffer.putShort((short) (this.rectangle.top_left_x * this.scaleWidth));
			buffer.putShort((short) (this.rectangle.top_left_y * this.scaleHeight));
			buffer.putShort((short) (this.rectangle.bottom_right_x * this.scaleWidth));
			buffer.putShort((short) (this.rectangle.bottom_right_y * this.scaleHeight));
		} else {
			buffer.putShort(this.rectangle.top_left_x);
			buffer.putShort(this.rectangle.top_left_y);
			buffer.putShort(this.rectangle.bottom_right_x);
			buffer.putShort(this.rectangle.bottom_right_y);
		}
		
		// put fuzzy byte
		buffer.put(BoxType.FUZZY);
	}

	/* (non-Javadoc)
	 * @see org.ocera.orte.types.MessageData#getMaxDataLength()
	 */
	@Override
	public int getMaxDataLength()
	{
		return ORTEConstant.BYTE_FIELD_SIZE + ORTEConstant.LONG_FIELD_SIZE + 4*ORTEConstant.SHORT_FIELD_SIZE;
	}
	
	/**
	 * When screen rotates, change scale variables to
	 * fit the destination screen.
	 * 
	 * Method of former Box class.
	 * 
	 * @since 1.0
	 */
	public void setScale(int currentWidth, int currentHeight)
	{
		this.scaleWidth = DESTINATION_WIDTH / currentWidth;
		this.scaleHeight = DESTINATION_HEIGHT / currentHeight;
	}
	
	/**
	 * Object parameters to be sent throw ORTE.
	 * 
	 * Class from former Box class.
	 * 
	 * @since 1.0
	 */
	public class BoxRect
	{
		public short top_left_x;
		public short top_left_y;
		public short bottom_right_x;
		public short bottom_right_y;
		
		public BoxRect()
		{}
	}
}
