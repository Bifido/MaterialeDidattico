using UnityEngine;
using System.Collections;

public class MyInputSwipe : MyInputBase {

	public override void Init(){
		base.Init();
		
		m_aoTouchInfos = new TouchInfo[mk_iMaxTouchNumber];
		
		InitTouch();
		
		Debug.Log("Initiating MyInputSwipe.cs");
	}

//	public override void InputUpdate(){
//		base.InputUpdate();
//		
//		if(Input.GetMouseButtonDown(0)){
//			this.m_vSwipeStartPosition =  Camera.main.ScreenToWorldPoint(Input.mousePosition);
//			this.m_vSwipeStartPosition.z = 0;
//		}
//
//
//		if(Input.GetMouseButtonUp(0)){
//			this.m_vSwipeEndPosition = Camera.main.ScreenToWorldPoint(Input.mousePosition);
//			this.m_vSwipeEndPosition.z = 0;
//			if(Mathf.Abs(this.m_vSwipeStartPosition.x)<1f && 
//			   this.m_vSwipeStartPosition.y<-2f && this.m_vSwipeStartPosition.y>-4f &&
//			   this.m_vSwipeEndPosition.y>-4f &&
//			   Vector3.Distance(this.m_vSwipeStartPosition,this.m_vSwipeEndPosition) > 1.5f){
//				Debug.DrawLine(this.m_vSwipeStartPosition,this.m_vSwipeEndPosition,Color.red);
//				base.InternalShootDetected(this.m_vSwipeEndPosition);
//			}
//		}
//	}

	public override void InputUpdate(){
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
				iTouches |= 1 << Input.touches[i].fingerId;
				
				Vector3 vPos2 = Camera.main.ScreenToWorldPoint(Input.touches[i].position);
				vPos2.z = 0f;
				
				//				Vector3 vPos = Input.touches[i].position;
				//				vPos.x /= Screen.width;
				//				vPos.y /= Screen.height;

				m_aoTouchInfos[(short)Input.touches[i].fingerId].m_oContInput.AddPosition(vPos2,Time.deltaTime);
			}
		}
		
		for(short i = 0; i < mk_iMaxTouchNumber; i++){
			if(m_aoTouchInfos[i].m_bStarted){
				//Check Gestures..
				CheckGesture(i);
				
				//Touch finished..
				if((iTouches & 1 << i) == 0){
					TouchFinished(i);
				}
			}
		}
		
		//mouse test
		//		if(Input.GetMouseButton(0)){
		//			m_aoTouchInfos[0].m_bStarted = true;
		//			
		//			//Update the touch position
		//			iTouches |= 1 << 0;
		//
		//			Vector3 vPos2 = Camera.main.ScreenToWorldPoint(Input.mousePosition);
		//			vPos2.z = 0f;
		//			Debug.Log("ScreeToWorld pos: "+ vPos2);
		////			vPos.x /= Screen.width;
		////			vPos.y /= Screen.height;
		//			
		//			m_aoTouchInfos[0].m_oContInput.AddPosition(vPos2, Time.deltaTime);
		//			base.InternalTouchChanged(0,vPos2);
		//
		//			base.InternalTouchesCountChanged(1);
		//		}else{
		//			base.InternalTouchesCountChanged(0);
		//		}
	}
	private void TouchFinished(int iID){
		float fTime = 0.0f;
		float fDistance = 0.0f;
		Vector3 vDirection = Vector3.zero;
		
		Vector3 vStartPos;
		Vector3 vEndPos;
		
		m_aoTouchInfos[iID].m_oContInput.GetGestureStatus(out vStartPos,out vEndPos, out fDistance, out fTime, out vDirection);
		
		float fSpeed = fDistance / fTime;
		float fAngle = VectorUtils.Angle(mk_vReferenceDirection, mk_vUpVector, vDirection);
		bool bValidAngle = VectorUtils.IsAngleWithinThreshold(vDirection, mk_vUpVector, mk_vReferenceDirection, mk_iAngleTreshold);
		
		Debug.Log("RELEASED GESTURE ("+iID+"): vStartPos = "+vStartPos+" vEndPos = "+vEndPos+" fDistance = " + fDistance + "fSpeed = " + fSpeed + " bValidAngle = " + bValidAngle + " fAngle = " + fAngle);

		switch(Constants.WEAPON){
		case 0:
			if(Mathf.Abs(vStartPos.x)<1f && 
			   vStartPos.y<-2f && 
			   vStartPos.y>-4f &&
			   vEndPos.y>-4f &&
			   fDistance > 1.5f){
					Debug.DrawLine(vStartPos,vEndPos,Color.red);
					base.InternalShootDetected(vEndPos);
			}
			break;
		default:
			Debug.Log("Wrong weapon index: "+(int)Constants.WEAPON);
			break;
		}

		m_aoTouchInfos[iID].m_oContInput.Clear();		
		m_aoTouchInfos[iID].m_bStarted = false;
	}
	
	//not required now
	private void CheckGesture(int iID){
		Debug.Log("Checking if is a "+Constants.WEAPON.ToString()+" gesture");

		float fTime = 0.0f;
		float fDistance = 0.0f;
		Vector3 vDirection = Vector3.zero;

		Vector3 vStartPos;
		Vector3 vEndPos;

		m_aoTouchInfos[iID].m_oContInput.GetGestureStatus(out vStartPos,out vEndPos, out fDistance, out fTime, out vDirection);

		float fSpeed = fDistance / fTime;
		float fAngle = VectorUtils.Angle(mk_vReferenceDirection, mk_vUpVector, vDirection);
		bool bValidAngle = VectorUtils.IsAngleWithinThreshold(vDirection, mk_vUpVector, mk_vReferenceDirection, mk_iAngleTreshold);
		
		Debug.Log("CHECKING GESTURE ("+iID+"): vStartPos = "+vStartPos+" vEndPos = "+vEndPos+" fDistance = " + fDistance + "fSpeed = " + fSpeed + " bValidAngle = " + bValidAngle + " fAngle = " + fAngle);

		switch(Constants.WEAPON){
			case 0:
//				if(	Mathf.Abs(vStartPos.x)<1f && 
//				   	vStartPos.y<-2f && 
//				   	vStartPos.y>-4f){
//					Debug.Log("Initial pos Valid");
//				}else{
//					Debug.Log("Initial pos NOT Valid");
//				}
				//need to check direction doesn't change to much?
				break;
			default:
				Debug.Log("Wrong weapon index: "+(int)Constants.WEAPON);
			break;
		}

//		if(fDistance >= mk_fMinDistanceForValidate && fSpeed > mk_fMinSpeedForValidate){
//			float fAngle = VectorUtils.Angle(mk_vReferenceDirection, mk_vUpVector, vDirection);
//			bool bValidAngle = VectorUtils.IsAngleWithinThreshold(vDirection, mk_vUpVector, mk_vReferenceDirection, mk_iAngleTreshold);
//			
//			Debug.Log("fDistance = " + fDistance + "fSpeed = " + fSpeed + " bValidAngle = " + bValidAngle + " fAngle = " + fAngle + " iID = " + iID);
//			
//			if(bValidAngle)			{
//				InternalShootDetected();
//				
//				//Clear the gesture info..
//				m_aoTouchInfos[iID].m_oContInput.Clear();
//				
//				//TODO: Go to the next gesture to check
//			}
//		}
	}

	private void InitTouch(){
		for(int i = 0; i < mk_iMaxTouchNumber; ++i){
			m_aoTouchInfos[i].m_oContInput = new MyContinuousInput(mk_iTouchWindowSize);
			m_aoTouchInfos[i].m_bStarted = false;
		}
	}

	//VARS
	private struct TouchInfo{
		public MyContinuousInput 	m_oContInput;
		public bool 			m_bStarted;
	}
	
	private TouchInfo[] 		m_aoTouchInfos;
	
	private const int 			mk_iMaxTouchNumber = 1;
	private const int 			mk_iTouchWindowSize = 20;
	
	private const int 			mk_iAngleTreshold = 30;
	private const float 		mk_fMinSpeedForValidate = 0.5f;
	private const float 		mk_fMinDistanceForValidate = 0.15f;
	private readonly Vector3 	mk_vReferenceDirection = new Vector3(0.0f, 1.0f, 0.0f);
	private readonly Vector3 	mk_vUpVector = new Vector3(0.0f, 0.0f, 1.0f);
}
