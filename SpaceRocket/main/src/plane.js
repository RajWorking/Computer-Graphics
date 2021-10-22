import * as THREE from 'three';
import Bullet from './bullets';
import { scene, camera } from './index';
import GameObject from './object';

export default class Plane extends GameObject {
    constructor(position, body = '/models/monkey.gltf', scale = 1) {
        super(position, body, scale);
        this.bullets = [];
    }

    checkScreen() {
        var box_size = this.getBox().getSize(new THREE.Vector3());
        // console.log(box_size, camera.position);

        var vFOV = THREE.MathUtils.degToRad(camera.fov); // convert vertical fov to radians
        var height =
            Math.tan(vFOV / 2) * (camera.position.z - this.Mesh.position.z); // visible height
        var width = height * camera.aspect; // visible width

        // console.log(height, width);

        this.Mesh.position.x = Math.max(
            camera.position.x - width + box_size.x / 2,
            this.Mesh.position.x
        );
        this.Mesh.position.x = Math.min(
            camera.position.x + width - box_size.x / 2,
            this.Mesh.position.x
        );

        this.Mesh.position.y = Math.max(
            camera.position.y - height + box_size.y / 2,
            this.Mesh.position.y
        );
        this.Mesh.position.y = Math.min(
            camera.position.y + height - box_size.y / 2,
            this.Mesh.position.y
        );
        this.Mesh.position.z = Math.max(
            10,
            this.Mesh.position.z
        );
        this.Mesh.position.z = Math.min(
            50,
            this.Mesh.position.z
        );
    }

    fire() {
        var box_size = this.getBox().getSize(new THREE.Vector3());
        var fire_position = { ...this.Mesh.position };
        fire_position.x -= 1;
        var new_bullet_left = new Bullet([fire_position.x, fire_position.y, fire_position.z]);
        fire_position.x += 2;
        var new_bullet_right = new Bullet([fire_position.x, fire_position.y, fire_position.z]);
        
        this.bullets.push(new_bullet_left);
        this.bullets.push(new_bullet_right);

    }
}
