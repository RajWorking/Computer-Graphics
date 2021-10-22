import * as THREE from 'three';
import { GLTFLoader } from 'three/examples/jsm/loaders/GLTFLoader';
import { scene } from './index';

export default class GameObject {
    constructor(position, body, scale) {
        this.body = body;
        this.scale = scale;
        this.x = position[0];
        this.y = position[1];
        this.z = position[2];

        this.loadGLTF();
    }

    getBox() {
        return new THREE.Box3().setFromObject(this.Mesh);
    }

    loadGLTF() {
        let balloonLoader = new GLTFLoader();

        balloonLoader.load(this.body, gltf => {
            this.Mesh = gltf.scene;
            this.Mesh.scale.set(this.scale, this.scale, this.scale);
            scene.add(this.Mesh);

            // this.box = new THREE.BoxHelper(this.Mesh, 0xffff00);
            // scene.add(this.box);

            this.Mesh.position.x = this.x;
            this.Mesh.position.y = this.y;
            this.Mesh.position.z = this.z;
        });
    }
}
