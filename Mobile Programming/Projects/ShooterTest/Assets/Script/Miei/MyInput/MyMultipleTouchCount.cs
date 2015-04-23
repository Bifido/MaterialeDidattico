using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MyMultipleTouchCount : MyInputBase {

	public override void Init(){
		base.Init();
		
		m_aoTouchInfos = new TouchInfo[mk_iMaxTouchNumber];
		
		InitTouch();

		Debug.Log("Starting MyMultipleTouchCount.cs");
	}
	
	private void InitTouch(){
		for(int i = 0; i < mk_iMaxTouchNumber; ++i){
			m_aoTouchInfos[i].m_oContInput = new ContinuousInput(mk_iTouchWindowSize);
			m_aoTouchInfos[i].m_bStarted = false;
		}
	}
	
	public override void InputUpdate(){
		Debug.Log("Calling MyMultipleTouchCount update");
		base.InputUpdate();
		
		int iTouches = 0;
		
		for(int i = 0; i < Input.touchCount; ++i){
			if(Input.touches[i].fingerId >= mk_iMaxTouchNumber){
				Debug.LogError("Finger ID excedes max touch numbers");
				return;
			}
			else{
				m_aoTouchInfos[Input.touches[i].fingerId].m_bStarted = true;
				
				//Update the touch position
				iTouches |= 1 << i;
				short iID = (short)Input.touches[i].fingerId;
				
				Vector3 vPos = Input.touches[i].position;
				vPos.x /= Screen.width;
				vPos.y /= Screen.height;
				
				m_aoTouchInfos[iID].m_oContInput.AddPosition(vPos, Time.deltaTime);
				base.InternalTouchChanged(iID,vPos);
			}
		}

		short NumOfDifferentTouch = 0;
		for(short i = 0; i < mk_iMaxTouchNumber; ++i){
			if(m_aoTouchInfos[i].m_bStarted){
				//Check Gestures..
//				CheckGesture(i);
				
				//Touch finished..
				if((iTouches & 1 << i) == 0){
					TouchFinished(i);
					base.InternalTouchEnd(i);
					NumOfDifferentTouch--;
				}else{
					NumOfDifferentTouch++;
				}
			}
		}
		base.InternalTouchesCountChanged(NumOfDifferentTouch);

		//mouse test
		if(Input.GetMouseButton(0)){
			base.InternalTouchesCountChanged(1);

			m_aoTouchInfos[0].m_bStarted = true;
			
			//Update the touch position
			iTouches |= 1 << 0;
			
			Vector3 vPos = Input.mousePosition;
			vPos.x /= Screen.width;
			vPos.y /= Screen.height;
			
			m_aoTouchInfos[0].m_oContInput.AddPosition(vPos, Time.deltaTime);
			base.InternalTouchChanged(0,vPos);
		}else{
			base.InternalTouchesCountChanged(0);
		}
	}
	
	
	private void TouchFinished(int iID){
		m_aoTouchInfos[iID].m_oContInput.Clear();		
		m_aoTouchInfos[iID].m_bStarted = false;
	}

	//not required now
//	private void CheckGesture(int iID){
//		float fTime = 0.0f;
//		float fDistance = 0.0f;
//		Vector3 vDirection = Vector3.zero;
//		
//		m_aoTouchInfos[iID].m_oContInput.GetGestureStatus(out fDistance, out fTime, out vDirection);
//		
//		float fSpeed = fDistance / fTime;
//		if(fDistance >= mk_fMinDistanceForValidate && fSpeed > mk_fMinSpeedForValidate){
//			float fAngle = VectorUtils.Angle(mk_vReferenceDirection, mk_vUpVector, vDirection);
//			bool bValidAngle = VectorUtils.IsAngleWithinThreshold(vDirection, mk_vUpVector, mk_vReferenceDirection, mk_iAngleTreshold);
//			
//			Debug.Log("fDistance = " + fDistance + "fSpeed = " + fSpeed + " bValidAngle = " + bValidAngle + " fAngle = " + fAngle + " iID = " + iID);
//			
//			if(bValidAngle)
//			{
//				InternalShootDetected();
//				
//				//Clear the gesture info..
//				m_aoTouchInfos[iID].m_oContInput.Clear();
//				
//				//TODO: Go to the next gesture to check
//			}
//		}
//	}
	
	//VARS
	private struct TouchInfo{
		public ContinuousInput 	m_oContInput;
		public bool 			m_bStarted;
	}
	
	private TouchInfo[] 		m_aoTouchInfos;
	
	private const int 			mk_iMaxTouchNumber = 10;
	private const int 			mk_iTouchWindowSize = 15;
	
	private const int 			mk_iAngleTreshold = 30;
	private const float 		mk_fMinSpeedForValidate = 0.5f;
	private const float 		mk_fMinDistanceForValidate = 0.15f;
	private readonly Vector3 	mk_vReferenceDirection = new Vector3(0.0f, 1.0f, 0.0f);
	private readonly Vector3 	mk_vUpVector = new Vector3(0.0f, 0.0f, 1.0f);
}
