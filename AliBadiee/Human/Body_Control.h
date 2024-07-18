#include "Device_Draw.h"

// last move for w and s
static string last_move = "";
static boolean positive = true;

// degree of hands and legs
static float walk_degree_forward_backward = 0.0f; // y

// speed of walking
static float speed = (3.0f) / 100.0f;

// walkTo ability position
float walk_to_direction[3] = {3.4f, 7.6f, 10.0f};

void resetBodyByPosition() {
    //
    // Function to reset positions of the body to 90 degrees when it reaches a wall,
    //

    // find start index of human objects
    int hand = any_cast<int>(human_staff["hand"]);
    int leg = any_cast<int>(human_staff["leg"]);
    int foot = any_cast<int>(human_staff["foot"]);
    int finger = any_cast<int>(human_staff["finger"]);


    //
    // Reset each object to default status and position
    //

    // hands
    human_worlds[hand] = human.positionFinder(3.0f, 0,
                                              new float[]{-1.9f - walk_position_left_right, -3.5f + walk_position_up_down,
                                                          0.0f - walk_position_forward_backward}, 3.14 / 2, 0, "x",
                                              "x");
    human_worlds[hand + 1] = human.positionFinder(3.0f, 0,
                                                  new float[]{+1.9f - walk_position_left_right, -3.5f + walk_position_up_down,
                                                              0.0f - walk_position_forward_backward}, 3.14 / 2, 0, "x",
                                                  "x");

    // legs
    human_worlds[leg] = human.positionFinder(3.0f, 0,
                                             new float[]{-1.2f - walk_position_left_right, -5.8f + walk_position_up_down,
                                                         0.0f - walk_position_forward_backward}, 3.14 / 2, 0, "x", "x");
    human_worlds[leg + 1] = human.positionFinder(3.0f, 0,
                                                 new float[]{1.2f - walk_position_left_right, -5.8f + walk_position_up_down,
                                                             0.0f - walk_position_forward_backward}, 3.14 / 2, 0, "x",
                                                 "x");

    // feet
    human_worlds[foot] = human.positionFinder(0.15, 3,
                                              new float[]{1.2f - walk_position_left_right, -7.35f + walk_position_up_down,
                                                          -0.1f - walk_position_forward_backward}, 3.14 / 2, 0, "x",
                                              "x");
    human_worlds[foot + 1] = human.positionFinder(0.15, 3,
                                                  new float[]{-1.2f - walk_position_left_right,
                                                              -7.35f + walk_position_up_down,
                                                              -0.1f - walk_position_forward_backward}, 3.14 / 2, 0, "x",
                                                  "x");

    // left fingers
    human_worlds[finger] = human.positionFinder(0.15f, 2.9f,
                                                new float[]{-1.93f - walk_position_left_right, -5.05f + walk_position_up_down,
                                                            +0.10f - walk_position_forward_backward}, 3.14 / 2, 0, "x",
                                                "x");
    human_worlds[finger + 1] = human.positionFinder(0.2f, 2.9f,
                                                    new float[]{-1.99f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                +0.05f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 2] = human.positionFinder(0.25f, 2.9f,
                                                    new float[]{-2.0f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.02f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 3] = human.positionFinder(0.23f, 2.9f,
                                                    new float[]{-1.96f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.08f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 4] = human.positionFinder(0.21f, 2.9f,
                                                    new float[]{-1.90f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.10f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");

    // right fingers
    human_worlds[finger + 5] = human.positionFinder(0.15f, 2.9f,
                                                    new float[]{1.93f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                +0.10f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 6] = human.positionFinder(0.2f, 2.9f,
                                                    new float[]{1.99f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                +0.05f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 7] = human.positionFinder(0.25f, 2.9f,
                                                    new float[]{2.0f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.02f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 8] = human.positionFinder(0.23f, 2.9f,
                                                    new float[]{1.96f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.08f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
    human_worlds[finger + 9] = human.positionFinder(0.21f, 2.9f,
                                                    new float[]{1.90f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.10f - walk_position_forward_backward}, 3.14 / 2, 0,
                                                    "x", "x");
}

boolean inHouse() {
    //
    // The Function to check if the human is in the house or not
    //

    return v[any_cast<int>(vertex_indexes["house_door"])]._y < plane_y + world.roof_height - 1.5f;
}

void walkForBack(float degreeToRotate, string direction) { // z
    //
    // Function to walk forward and backward in Z ax,
    //
    // Note: by pressing keys "w" and "s"
    //

    // find start index of human objects
    int hand = any_cast<int>(human_staff["hand"]);
    int leg = any_cast<int>(human_staff["leg"]);
    int foot = any_cast<int>(human_staff["foot"]);
    int finger = any_cast<int>(human_staff["finger"]);

    // when human walks, move the door (close or open the garage door)
    world.createDoor(walk_position_forward_backward, "move");

    // prevent from overflowing the house
    if (inHouse())
        if ((direction == "w" && walk_position_forward_backward > plane_z - 1) ||
            (direction == "s" && walk_position_forward_backward < -plane_z + 1)) {

            resetBodyByPosition();
            return;
        }

    // change the position with speed
    if (direction == "w") walk_position_forward_backward += speed;
    if (direction == "s") walk_position_forward_backward -= speed;


    // legs new position
    human_worlds[leg] = human.positionFinder(3.0f, 0,
                                             new float[]{-1.2f - walk_position_left_right, -5.8f + walk_position_up_down,
                                                         0.0f - walk_position_forward_backward}, 3.14 / 2,
                                             degreeToRotate, "x", "x");
    human_worlds[leg + 1] = human.positionFinder(3.0f, 0,
                                                 new float[]{1.2f - walk_position_left_right, -5.8f + walk_position_up_down,
                                                             0.0f - walk_position_forward_backward}, 3.14 / 2,
                                                 -degreeToRotate, "x", "x");

    // draw the new legs with rotation
    Device->SetTransform(D3DTS_WORLD, &human_worlds[leg]);
    human_objects[leg]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[leg + 1]);
    human_objects[leg + 1]->DrawSubset(0);


    // feet new position
    human_worlds[foot] = human.positionFinder(0.15, 3,
                                              new float[]{1.2f - walk_position_left_right, -7.35f + walk_position_up_down,
                                                          -0.1f - walk_position_forward_backward}, 3.14 / 2,
                                              -degreeToRotate, "x",
                                              "x");
    human_worlds[foot + 1] = human.positionFinder(0.15, 3,
                                                  new float[]{-1.2f - walk_position_left_right,
                                                              -7.35f + walk_position_up_down,
                                                              -0.1f - walk_position_forward_backward}, 3.14 / 2,
                                                  degreeToRotate,
                                                  "x", "x");

    // draw the new feet with rotation
    Device->SetTransform(D3DTS_WORLD, &human_worlds[foot]);
    human_objects[foot]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[foot + 1]);
    human_objects[foot + 1]->DrawSubset(0);


    // hands new position
    human_worlds[hand] = human.positionFinder(3.0f, 0,
                                              new float[]{-1.9f - walk_position_left_right, -3.5f + walk_position_up_down,
                                                          0.0f - walk_position_forward_backward}, 3.14 / 2,
                                              degreeToRotate, "x",
                                              "x");
    human_worlds[hand + 1] = human.positionFinder(3.0f, 0,
                                                  new float[]{+1.9f - walk_position_left_right, -3.5f + walk_position_up_down,
                                                              0.0f - walk_position_forward_backward}, 3.14 / 2,
                                                  -degreeToRotate,
                                                  "x", "x");

    // draw the new hands with rotation
    Device->SetTransform(D3DTS_WORLD, &human_worlds[hand]);
    human_objects[hand]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[hand + 1]);
    human_objects[hand + 1]->DrawSubset(0);


    // fingers new position
    human_worlds[finger] = human.positionFinder(0.15f, 2.9f,
                                                new float[]{-1.93f - walk_position_left_right, -5.05f + walk_position_up_down,
                                                            +0.10f - walk_position_forward_backward}, 3.14 / 2,
                                                degreeToRotate,
                                                "x", "x");
    human_worlds[finger + 1] = human.positionFinder(0.2f, 2.9f,
                                                    new float[]{-1.99f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                +0.05f - walk_position_forward_backward}, 3.14 / 2,
                                                    degreeToRotate,
                                                    "x", "x");
    human_worlds[finger + 2] = human.positionFinder(0.25f, 2.9f,
                                                    new float[]{-2.0f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.02f - walk_position_forward_backward}, 3.14 / 2,
                                                    degreeToRotate,
                                                    "x", "x");
    human_worlds[finger + 3] = human.positionFinder(0.23f, 2.9f,
                                                    new float[]{-1.96f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.08f - walk_position_forward_backward}, 3.14 / 2,
                                                    degreeToRotate,
                                                    "x", "x");
    human_worlds[finger + 4] = human.positionFinder(0.21f, 2.9f,
                                                    new float[]{-1.90f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.10f - walk_position_forward_backward}, 3.14 / 2,
                                                    degreeToRotate,
                                                    "x", "x");

    human_worlds[finger + 5] = human.positionFinder(0.15f, 2.9f,
                                                    new float[]{1.93f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                +0.10f - walk_position_forward_backward}, 3.14 / 2,
                                                    -degreeToRotate, "x",
                                                    "x");
    human_worlds[finger + 6] = human.positionFinder(0.2f, 2.9f,
                                                    new float[]{1.99f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                +0.05f - walk_position_forward_backward}, 3.14 / 2,
                                                    -degreeToRotate, "x",
                                                    "x");
    human_worlds[finger + 7] = human.positionFinder(0.25f, 2.9f,
                                                    new float[]{2.0f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.02f - walk_position_forward_backward}, 3.14 / 2,
                                                    -degreeToRotate, "x",
                                                    "x");
    human_worlds[finger + 8] = human.positionFinder(0.23f, 2.9f,
                                                    new float[]{1.96f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.08f - walk_position_forward_backward}, 3.14 / 2,
                                                    -degreeToRotate, "x",
                                                    "x");
    human_worlds[finger + 9] = human.positionFinder(0.21f, 2.9f,
                                                    new float[]{1.90f - walk_position_left_right,
                                                                -5.05f + walk_position_up_down,
                                                                -0.10f - walk_position_forward_backward}, 3.14 / 2,
                                                    -degreeToRotate, "x",
                                                    "x");

    // draw the new fingers with rotation
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger]);
    human_objects[finger]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 1]);
    human_objects[finger + 1]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 2]);
    human_objects[finger + 2]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 3]);
    human_objects[finger + 3]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 4]);
    human_objects[finger + 4]->DrawSubset(0);

    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 5]);
    human_objects[finger + 5]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 6]);
    human_objects[finger + 6]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 7]);
    human_objects[finger + 7]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 8]);
    human_objects[finger + 8]->DrawSubset(0);
    Device->SetTransform(D3DTS_WORLD, &human_worlds[finger + 9]);
    human_objects[finger + 9]->DrawSubset(0);


    // translate all objects by a "speed" unit.
    if (direction == "w")
        for (int i = 0; i < human_object_count; i++)
            human_worlds[i] = human.translate(human_worlds[i], i, 0.0f, 0.0f, -speed);

    else
        for (int i = 0; i < human_object_count; i++)
            human_worlds[i] = human.translate(human_worlds[i], i, 0.0f, 0.0f, speed);
}

void walkUpDown(string direction) { // y
    //
    // Function to walk forward and backward in Y ax,
    //
    // Note: by pressing keys "q" and "e"
    //

    // prevent from overflowing the plane
    if ((direction == "q" && walk_position_up_down > 9) || (direction == "e" && walk_position_up_down < 0.1))
        return;

    // change the position with speed
    if (direction == "q") walk_position_up_down += speed;
    if (direction == "e") walk_position_up_down -= speed;


    // translate all objects by a "speed" unit.
    if (direction == "q")
        for (int i = 0; i < human_object_count; i++)
            human_worlds[i] = human.translate(human_worlds[i], i, 0.0f, +speed, 0.0f);

    else
        for (int i = 0; i < human_object_count; i++)
            human_worlds[i] = human.translate(human_worlds[i], i, 0.0f, -speed, 0.0f);
}

void walkLeftRight(string direction) {
    //
    // Function to walk forward and backward in X ax,
    //
    // Note: by pressing keys "a" and "d"
    //

    // prevent from overflowing the plane
    if (inHouse())
        if ((direction == "a" && walk_position_left_right > plane_x - 2) ||
            (direction == "d" && walk_position_left_right < -plane_x + 2))
            return;

    // change the position with speed
    if (direction == "a") walk_position_left_right += speed;
    if (direction == "d") walk_position_left_right -= speed;


    // translate all objects by a "speed" unit.
    if (direction == "a")
        for (int i = 0; i < human_object_count; i++)
            human_worlds[i] = human.translate(human_worlds[i], i, -speed, 0.0f, 0.0f);

    else
        for (int i = 0; i < human_object_count; i++)
            human_worlds[i] = human.translate(human_worlds[i], i, speed, 0.0f, 0.0f);
}

void walkTo() {
    //
    // Function to get a direction and walk to that point
    //

    // walk x
    if (walk_position_left_right - speed < walk_to_direction[0]) // x
        walkLeftRight("a");

    if (walk_position_left_right + speed > walk_to_direction[0])
        walkLeftRight("d");

    // walk y
    if (walk_position_up_down - speed < walk_to_direction[1]) // y
        walkUpDown("q");

    if (walk_position_up_down + speed > walk_to_direction[1])
        walkUpDown("e");

    // check if z is reached and prevent legs and hands to hang
    if (walk_to_direction[2] - speed < walk_position_forward_backward &&
        walk_position_forward_backward < walk_to_direction[2] + speed) {
        resetBodyByPosition();
        return;
    }

    // walk z
    if (walk_to_direction[2] > walk_position_forward_backward) {// z
        if (positive)
            walk_degree_forward_backward += speed;

        else
            walk_degree_forward_backward -= speed;

        walkForBack(walk_degree_forward_backward, "w");

        if (walk_degree_forward_backward >= 3.14f / 4)
            positive = false;

        else if (walk_degree_forward_backward <= -3.14f / 4)
            positive = true;
    }

    if (walk_to_direction[2] < walk_position_forward_backward) {
        if (positive)
            walk_degree_forward_backward += speed / 2.0f;

        else
            walk_degree_forward_backward -= speed / 2.0f;

        walkForBack(-walk_degree_forward_backward, "s");

        if (walk_degree_forward_backward >= 3.14f / 6)
            positive = false;

        else if (walk_degree_forward_backward <= -3.14f / 6)
            positive = true;
    }
}
