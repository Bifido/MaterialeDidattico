using UnityEngine;
using System.Collections;

#if UNITY_EDITOR
using QualitySpecific = Quality_Editor;
#elif UNITY_IPHONE
using QualitySpecific = Quality_iOS;
#elif UNITY_ANDROID
using QualitySpecific = Quality_Android;
#elif UNITY_STANDALONE
using QualitySpecific = Quality_Standalone;
#else
using QualitySpecific = Quality_Editor;
#endif

public class Quality 
{
	public static bool UseHightQuality()
	{
		return QualitySpecific.UseHightQuality();
	}

	public static eQualityLevel GetQualityLevel()
	{
		if(m_eQualityLevel == eQualityLevel.NONE)
		{
			m_eQualityLevel = QualitySpecific.GetQualityLevel();
		}

		return m_eQualityLevel;
	}

	public static string GetQualitySuffix()
	{
		if(UseHightQuality())
		{
			return m_sQualitySuffix;
		}
		
		return "";
	}

	public enum eQualityLevel
	{
		NONE = 0,
		BASE,
		MEDIUM,
		HIGHT,
		BEST
	};

	private static eQualityLevel m_eQualityLevel = eQualityLevel.NONE;

	private static string m_sQualitySuffix = "-hd";
}
