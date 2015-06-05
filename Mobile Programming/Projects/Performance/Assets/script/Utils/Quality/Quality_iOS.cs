using UnityEngine;
using System.Collections;

public class Quality_iOS 
{
	public static bool UseHightQuality()
	{
		#if UNITY_IPHONE
		if(UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPad1Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPad2Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPadMini1Gen //iPad
		   || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone3G || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone3GS || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone4 || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone4S //iPhone
		   || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch1Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch2Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch3Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch4Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch5Gen) //iPodTouch
		{
			return false;
		}
		#endif
		return true;
	}

	public static Quality.eQualityLevel GetQualityLevel()
	{
		#if UNITY_IPHONE
		if(UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPad1Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPad2Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPadMini1Gen//iPad
		   || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone3G || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone3GS || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone4 || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPhone4S //iPhone
		   || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch1Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch2Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch3Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch4Gen || UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPodTouch5Gen) //iPodTouch
		{
			return Quality.eQualityLevel.BASE;
		}
		else if(UnityEngine.iOS.Device.generation == UnityEngine.iOS.DeviceGeneration.iPad3Gen)
		{
			return Quality.eQualityLevel.MEDIUM;
		}
		else
		{
			return Quality.eQualityLevel.HIGHT;
		}

		#else
		return Quality.eQualityLevel.HIGHT;
		#endif
	}

}
