/**
 ******************************************************************************
 * @file       uavobjecttemplate.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @author     Tau Labs, http://taulabs.org, Copyright (C) 2012-2013
 * @brief      Template for an uavobject in java
 *             This is a autogenerated file!! Do not modify and expect a result.
 *             Maintains the telemetry statistics from the OpenPilot flight computer.
 *
 * @see        The GNU Public License (GPL) Version 3
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

package com.brainfpv.uavtalk.uavobjects;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;

import com.brainfpv.uavtalk.UAVDataObject;
import com.brainfpv.uavtalk.UAVObject;
import com.brainfpv.uavtalk.UAVObjectField;
import com.brainfpv.uavtalk.UAVObjectManager;

/**
Maintains the telemetry statistics from the OpenPilot flight computer.

generated from flighttelemetrystats.xml
 **/
public class FlightTelemetryStats extends UAVDataObject {

	public FlightTelemetryStats() {
		super(OBJID, ISSINGLEINST, ISSETTINGS, NAME);

		List<UAVObjectField> fields = new ArrayList<UAVObjectField>();


		List<String> TxDataRateElemNames = new ArrayList<String>();
		TxDataRateElemNames.add("0");
		fields.add( new UAVObjectField("TxDataRate", "bytes/sec", UAVObjectField.FieldType.FLOAT32, TxDataRateElemNames, null) );

		List<String> RxDataRateElemNames = new ArrayList<String>();
		RxDataRateElemNames.add("0");
		fields.add( new UAVObjectField("RxDataRate", "bytes/sec", UAVObjectField.FieldType.FLOAT32, RxDataRateElemNames, null) );

		List<String> TxFailuresElemNames = new ArrayList<String>();
		TxFailuresElemNames.add("0");
		fields.add( new UAVObjectField("TxFailures", "count", UAVObjectField.FieldType.UINT32, TxFailuresElemNames, null) );

		List<String> RxFailuresElemNames = new ArrayList<String>();
		RxFailuresElemNames.add("0");
		fields.add( new UAVObjectField("RxFailures", "count", UAVObjectField.FieldType.UINT32, RxFailuresElemNames, null) );

		List<String> TxRetriesElemNames = new ArrayList<String>();
		TxRetriesElemNames.add("0");
		fields.add( new UAVObjectField("TxRetries", "count", UAVObjectField.FieldType.UINT32, TxRetriesElemNames, null) );

		List<String> StatusElemNames = new ArrayList<String>();
		StatusElemNames.add("0");
		List<String> StatusEnumOptions = new ArrayList<String>();
		StatusEnumOptions.add("Disconnected");
		StatusEnumOptions.add("HandshakeReq");
		StatusEnumOptions.add("HandshakeAck");
		StatusEnumOptions.add("Connected");
		fields.add( new UAVObjectField("Status", "", UAVObjectField.FieldType.ENUM, StatusElemNames, StatusEnumOptions) );


		// Compute the number of bytes for this object
		int numBytes = 0;
		ListIterator<UAVObjectField> li = fields.listIterator();
		while(li.hasNext()) {
			numBytes += li.next().getNumBytes();
		}
		NUMBYTES = numBytes;

		// Initialize object
		initializeFields(fields, ByteBuffer.allocate(NUMBYTES), NUMBYTES);
		// Set the default field values
		setDefaultFieldValues();
		// Set the object description
		setDescription(DESCRIPTION);
	}

	/**
	 * Create a Metadata object filled with default values for this object
	 * @return Metadata object with default values
	 */
	@Override
	public Metadata getDefaultMetadata() {
		UAVObject.Metadata metadata = new UAVObject.Metadata();
    	metadata.flags =
		    UAVObject.Metadata.AccessModeNum(UAVObject.AccessMode.ACCESS_READWRITE) << UAVOBJ_ACCESS_SHIFT |
		    UAVObject.Metadata.AccessModeNum(UAVObject.AccessMode.ACCESS_READWRITE) << UAVOBJ_GCS_ACCESS_SHIFT |
		    1 << UAVOBJ_TELEMETRY_ACKED_SHIFT |
		    1 << UAVOBJ_GCS_TELEMETRY_ACKED_SHIFT |
		    UAVObject.Metadata.UpdateModeNum(UAVObject.UpdateMode.UPDATEMODE_PERIODIC) << UAVOBJ_TELEMETRY_UPDATE_MODE_SHIFT |
		    UAVObject.Metadata.UpdateModeNum(UAVObject.UpdateMode.UPDATEMODE_MANUAL) << UAVOBJ_GCS_TELEMETRY_UPDATE_MODE_SHIFT;
    	metadata.flightTelemetryUpdatePeriod = 5000;
    	metadata.gcsTelemetryUpdatePeriod = 0;
    	metadata.loggingUpdatePeriod = 5000;

		return metadata;
	}

	/**
	 * Initialize object fields with the default values.
	 * If a default value is not specified the object fields
	 * will be initialized to zero.
	 */
	public void setDefaultFieldValues()
	{

	}

	/**
	 * Create a clone of this object, a new instance ID must be specified.
	 * Do not use this function directly to create new instances, the
	 * UAVObjectManager should be used instead.
	 */
	@Override
	public UAVDataObject clone(long instID) {
		// TODO: Need to get specific instance to clone
		try {
			FlightTelemetryStats obj = new FlightTelemetryStats();
			obj.initialize(instID, this.getMetaObject());
			return obj;
		} catch  (Exception e) {
			return null;
		}
	}

	/**
	 * Static function to retrieve an instance of the object.
	 */
	public FlightTelemetryStats GetInstance(UAVObjectManager objMngr, long instID)
	{
	    return (FlightTelemetryStats)(objMngr.getObject(FlightTelemetryStats.OBJID, instID));
	}

	// Constants
	protected static final long OBJID = 0x2F7E2902l;
	protected static final String NAME = "FlightTelemetryStats";
	protected static String DESCRIPTION = "Maintains the telemetry statistics from the OpenPilot flight computer.";
	protected static final boolean ISSINGLEINST = 1 > 0;
	protected static final boolean ISSETTINGS = 0 > 0;
	protected static int NUMBYTES = 0;


}
