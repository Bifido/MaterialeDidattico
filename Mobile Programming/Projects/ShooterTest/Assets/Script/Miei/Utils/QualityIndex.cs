using UnityEngine;
using System.Collections;

public class QualityIndex{
	public Resolution 	m_Resolution;
	public float 		m_MemSize;

	QualityIndex(){
		this.m_Resolution = Screen.currentResolution;
		this.m_MemSize = SystemInfo.systemMemorySize;
		//todo etc etc
	}
}
