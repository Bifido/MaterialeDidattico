using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class MyMultipleTouchCount : MyInputBase {

	class TouchVector{
		public int TouchID { get; set; }
		public Vector2 StartPos { get; set; }
		public Vector2 LastPos { get; set; }
		public bool WasUpdatedThisFrame { get; set; }
	}
	
	public override void InputUpdate(){
		base.InputUpdate();		
		if (Input.GetMouseButtonDown(0)){
			base.InternalTouchesCountChanged(true);
//			foreach (Touch actual in Input.touches){
//				foreach (TouchVector actualSupport in m_oInputPending){
//					if (actualSupport.TouchID == actual.fingerId){
//						actualSupport.LastPos = actual.position;
//						actualSupport.WasUpdatedThisFrame = true;
//						alreadyIn = true;
//					}
//				}
//				
//				if(!alreadyIn && m_oInputPending.Count < MAX_TOUCH_COUNT){
//					m_oInputPending.Add(new TouchVector(){ 
//						TouchID = actual.fingerId, 
//						StartPos = actual.position, 
//						LastPos = actual.position, 
//						WasUpdatedThisFrame = true 
//					});
//				}
//			}
//			
//			List<TouchVector> notUpdatedThisFrame = m_oInputPending.Where(x => !x.WasUpdatedThisFrame).ToList();
//			
//			foreach (TouchVector actual in notUpdatedThisFrame)
//			{
//				if(RickySimpleGestureManager.EvaluteVectorConguency(actual.StartPos, actual.LastPos, RickySimpleGestureManager.SwipeDir.UP))
//				{
//					InternalJumpDetected();
//				}
//				if (RickySimpleGestureManager.EvaluteVectorConguency(actual.StartPos, actual.LastPos, RickySimpleGestureManager.SwipeDir.RIGHT))
//				{
//					InternalShootDetected();
//				}
//				
//				m_oInputPending.Remove(actual);
//			}
		}
	}

	private const int 			MAX_TOUCH_COUNT = 10;
	private	Vector3 			m_v3InputTouchCoord;
	private List<TouchVector> 	m_oInputPending = new List<TouchVector>(MAX_TOUCH_COUNT);
}
