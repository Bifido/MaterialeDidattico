using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class TextCounter : MonoBehaviour {

	// Use this for initialization
	void Start () {
		TextObj.text = m_sFixedText+m_iTouchCount;
	}

	// Update is called once per frame
	public void UpdateText(short touchCount) {
		m_iTouchCount = touchCount;
		TextObj.text = m_sFixedText+m_iTouchCount;
	}

	public Text				TextObj;

	private string 			m_sFixedText = "Touch Count: ";
	private int				m_iTouchCount = 0;
}
