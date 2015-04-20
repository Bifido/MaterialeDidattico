using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;

public class NativeServiceIOS
{
#if UNITY_IOS

	[DllImport ("__Internal")]
	private static extern void _Init ();

	//Init the service..
	public static void Init()
	{
		if (Application.platform != RuntimePlatform.OSXEditor)
			_Init();
	}

	[DllImport ("__Internal")]
	private static extern string _GetStatus();

	public static string GetStatus()
	{
		if (Application.platform != RuntimePlatform.OSXEditor)
			return _GetStatus();

		return "";
	}

	[DllImport ("__Internal")]
	private static extern int _GetIntValue();
	
	public static int GetInt()
	{
		if (Application.platform != RuntimePlatform.OSXEditor)
			return _GetIntValue();
		
		return 0;
	}

	[DllImport ("__Internal")]
	private static extern float _GetFloatValue();
	
	public static float GetFloat()
	{
		if (Application.platform != RuntimePlatform.OSXEditor)
			return _GetFloatValue();
		
		return 0.0f;
	}

	[DllImport ("__Internal")]
	private static extern bool _IsReady();
	
	public static bool IsReady()
	{
		if (Application.platform != RuntimePlatform.OSXEditor)
			return _IsReady();
		
		return false;
	}
#endif
}
