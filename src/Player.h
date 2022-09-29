#pragma once
#include "Utils.h"

// TO DO: SORT ALL THIS OUT, IT'S ONLY TEMPORARY TO VISUALIZE MY PATHFINDING
// REMEMEBER TO ADD SETTING CHECKS FOR DESTINATION ETC TO CHECK IT'S ACTAULLY A VALID DEST ETC ETC.

class Player 
{
    public:
        vec2 m_position = { 0 , 0 };
        vec2 m_destination = { 0, 0};
        bool m_playerReachedFinalDestination = true;
        std::vector<int> m_pathByBoxId = {};

        Player(float x, float y)
        {
            m_position = { x, y};
        }

        Player(){}

        void MovePlayer(int speed, vec2 destinationVec, float deltaTime)
        {
            vec2 moveDirection = { destinationVec.x - m_position.x, destinationVec.y - m_position.y};
            float lengthOfVec = sqrt((moveDirection.x)*(moveDirection.x) + (moveDirection.y)*(moveDirection.y));

            vec2 normalizedMoveDir = { (moveDirection.x)/lengthOfVec, (moveDirection.y)/lengthOfVec};
            m_position = { m_position.x += (normalizedMoveDir.x * speed * deltaTime), m_position.y += (normalizedMoveDir.y * speed * deltaTime) };
            //std::cout << "PLAYER POSITION: " << m_position.x << ", " << m_position.y << std::endl;

        }

        bool PlayerHasReachedBoxDestination()
        {
            if (m_position.x <= m_destination.x + 2  && m_position.x >= m_destination.x - 2 && m_position.y <= m_destination.y + 2 && m_position.y >= m_destination.y - 2)
            {
                //If we are near enough to dest, clamp to dest
                m_position.x = m_destination.x;
                m_position.y = m_destination.y;
                return true;
            }
            return false;
        }
};