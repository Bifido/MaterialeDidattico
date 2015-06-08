using UnityEngine;
using System.Collections;

public class Quality_Android  
{
	public static bool UseHightQuality()
	{
		return (Mathf.Max(Screen.width, Screen.height) > sk_iMinResolutionForHD);
	}

	public static Quality.eQualityLevel GetQualityLevel()
	{
		return Quality.eQualityLevel.HIGHT;
	}

	private static int sk_iMinResolutionForHD = 1280;
}
