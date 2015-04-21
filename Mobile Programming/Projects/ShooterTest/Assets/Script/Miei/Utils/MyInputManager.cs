using UnityEngine;
using System.Collections;

public class MyInputManager : MonoBehaviour {

	public delegate void InputEventEmpty();
	public event InputEventEmpty OnJump;
	public delegate void InputEvent(Vector3 endDir);
	public event InputEvent OnShoot;
	public delegate void InputEventBool(bool boolean);
	public event InputEventBool OnNewTouch;
	
	void Start(){
		InitInput();
	}
	
	void Update(){
		if(m_oInput != null){
			m_oInput.InputUpdate();
		}
	}
	
	private void JumpDetecet(){
		if(OnJump != null){
			OnJump();
		}
	}
	
	private void ShootDetected(Vector3 endDiretction){
		if(OnShoot != null){
			OnShoot(endDiretction);
		}
	}

	private void OnTouchCountChanged(bool boolean){
		if(OnNewTouch != null){
			OnNewTouch(boolean);
		}
	}
	
	private void InitInput(){
		m_oInput = MyInputFactory.GetInput(m_eInputSource);
		
		if(m_oInput != null){
			if(m_eInputSource == eMyInputSource.TOUCH_COUNTER){
				m_oInput.Activate(OnTouchCountChanged);
			}else{
				m_oInput.Activate(JumpDetecet, ShootDetected);
			}
		}
	}
	
	public void ChangeInput(eMyInputSource eNewInputSource){
		if(eNewInputSource != m_eInputSource){
			if(m_oInput != null){
				m_oInput.Deactivate();
				m_oInput = null;
			}
			
			m_eInputSource = eNewInputSource;
			
			InitInput(); 
		}
	}
	
	//VARS
	public enum eMyInputSource{
		PLAYER = 0,
		TOUCH_COUNTER,
		AI,
		REPLAY,
		NETWORK,
		COUNT
	}
	
	[SerializeField] private eMyInputSource m_eInputSource;
	
	private MyInputBase m_oInput;
}
