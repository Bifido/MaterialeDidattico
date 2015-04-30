using UnityEngine;
using System.Collections;

public class MyContinuousInput{

	public MyContinuousInput(int iArraySize){
		m_afDeltaTime = new float[iArraySize];
		m_avPositions = new Vector3[iArraySize];
		
		Clear();
	}
	
	public void Clear(){
		for(int i = 0; i < m_afDeltaTime.Length; ++i){
			m_afDeltaTime[i] = 0.0f;
			m_avPositions[i] = Vector3.zero;
		}
		
		m_iNextElement = 0;
		
		m_fElapsedTime = 0.0f;
		m_vFirstPosition = Vector3.zero;
		m_vStartPosition = Vector3.zero;
		m_vEndPosition = Vector3.zero;
	}

	public void ClearMaxDistance(){
		m_fMaxDistanceReached = 0f;
		m_vMaxDistancePos = Vector3.zero;
	}
	
	public void AddPosition(Vector3 vPosition, float fDT){
		if(m_vFirstPosition == Vector3.zero){
			m_vFirstPosition = vPosition;
		}

		m_fElapsedTime -= m_afDeltaTime[m_iNextElement];
		
		if(m_iNextElement == 0 && m_vStartPosition == Vector3.zero){
			m_vStartPosition = vPosition;
		}

		if((vPosition - m_vFirstPosition).magnitude > m_fMaxDistanceReached){
			m_fMaxDistanceReached = (vPosition - m_vFirstPosition).magnitude;
			m_vMaxDistancePos = vPosition;
		}
		
		m_afDeltaTime[m_iNextElement] = fDT;
		m_fElapsedTime += m_afDeltaTime[m_iNextElement];
		
		m_avPositions[m_iNextElement] = vPosition;
		m_vEndPosition = vPosition;
		
		m_iNextElement = (m_iNextElement + 1) % m_afDeltaTime.Length;
		
		if(m_avPositions[m_iNextElement] != Vector3.zero){
			m_vStartPosition = m_avPositions[m_iNextElement];
		}
	}
	
	public void GetGestureStatus(out float fDistance, out float fTime, out Vector3 vDirection){
		fTime = m_fElapsedTime;
		
		vDirection = m_vEndPosition - m_vStartPosition;
		
		fDistance = vDirection.magnitude;
		
		vDirection.Normalize();
	}

	public void GetGestureStatus(out Vector3 vStartPosition,out Vector3 vEndPosition, out float fDistance, out float fTime, out Vector3 vDirection){
		vStartPosition = m_vFirstPosition;

		vEndPosition = m_vEndPosition;

		fTime = m_fElapsedTime;
		
		vDirection = m_vEndPosition - m_vStartPosition;
		
		fDistance = vDirection.magnitude;
		
		vDirection.Normalize();

	}

	public void GetGestureStatusMaxDistance(out Vector3 vMaxDistancePos,out float fMaxDistanceReached){
		vMaxDistancePos = m_vMaxDistancePos;

		fMaxDistanceReached = m_fMaxDistanceReached;
	}
	
	//VARS
	private float		m_fMaxDistanceReached;
	private Vector3		m_vMaxDistancePos;
	private float[] 	m_afDeltaTime;
	private float 		m_fElapsedTime;
	private Vector3[] 	m_avPositions;
	private Vector3		m_vStartPosition;
	private Vector3		m_vEndPosition;
	private Vector3		m_vFirstPosition;
	private int 		m_iNextElement;
}
