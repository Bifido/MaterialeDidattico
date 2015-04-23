using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class TextCounter : MonoBehaviour {

	// Use this for initialization
	void Start () {
		TextObj.text = m_sFixedText+m_iTouchCount;
	}

	private void OnEnable(){
		if(MyInputManager == null){
			Debug.LogError("No InputManager in the scene");
			return;
		}		
		MyInputManager.OnNewTouch += UpdateText;
	}
	
	// Update is called once per frame
	void UpdateText(short touchCount) {
		m_iTouchCount = touchCount;
		TextObj.text = m_sFixedText+m_iTouchCount;
	}

	public Text				TextObj;
	public MyInputManager 	MyInputManager;

	private string 			m_sFixedText = "Touch Count: ";
	private int				m_iTouchCount = 0;
}
