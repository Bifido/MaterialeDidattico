
using UnityEngine;
using System.Collections;

public static class RickySimpleGestureManager {

    public enum SwipeDir
    {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        COUNT
    }

    public static bool EvaluteVectorConguency(Vector3 startVec, Vector3 endVec, SwipeDir where)
    {

        Vector3 vec = endVec - startVec;

        float cosAngle;

        cosAngle = Vector3.Dot(vec.normalized, m_av3Directions[(int) where]);
        return IsInsideCone(cosAngle);
    }

    private static bool IsInsideCone(float cosAngle)
    {
        // if the cos of the angle between is < 0 => nope
        if (cosAngle > 0)
        {
            float angle = Mathf.Acos(cosAngle);
            if (Mathf.Abs(angle) <= (Mathf.Deg2Rad * m_cfMaxAngle))
            {
                return true;
            }
        }

        return false;
    }


    static Vector3[] m_av3Directions = { Vector3.up, Vector3.down, Vector3.left, Vector3.right };

    const float m_cfMaxAngle = 45.0f;
}