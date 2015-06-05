using UnityEngine;
using System.Collections;

public class Quality_Editor : MonoBehaviour
{
	public static bool UseHightQuality()
	{
		return true;
	}

	public static Quality.eQualityLevel GetQualityLevel()
	{
		return Quality.eQualityLevel.HIGHT;
	}
}
