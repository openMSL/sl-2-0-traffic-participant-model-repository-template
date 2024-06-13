# SL-2-0 Traffic Participant Repository Template

[![Credibility Assessment](../../actions/workflows/credibility_assessment.yml/badge.svg)](https://github.com/openMSL/sl-2-0-traffic-participant-model-repository-template/actions/workflows/credibility_assessment.yml)

Enter a short description of the model.
What is the purpose of the model?
What is the general modeling approach?
What inputs does the model need and what outputs does it generate?
<br><br>

In this template, a very simple traffic participant is modeled.
The model consumes an osi3::SensorView and an osi3::TrafficCommand as input and outputs an osi3::TrafficUpdate and an osi3::TrafficCommandUpdate.
The TrafficUpdate at the output contains the update message with updated position and velocity data.

< Eye-catcher Image >
<img src="doc/img/model_video.gif" alt="Eye-catcher Image" width="800" />

## Modeling Approach

Put details about the inner workings of the model here.
Describe the modeling approach in detail.
What is the structure of the model?
What modules is it comprised of?
What sensor effects are represented in the model and how are they modeled?

## Parameterization

What parameters are used internally?
Which parameters can be set by the user?
Each parameter should have a short description as shown in the following example.

| Parameter     | Description                              |
|---------------|------------------------------------------|
| `parameter_a` | Some parameter to customize the model    |
| `parameter_b` | Another parameter to customize the model |

## Interface

What interfaces are used as model input and model output?
All required and optional messages of the interface shall be named in a list as shown in the following example.

### Input: Required Fields in osi3::SensorView and/or osi3::TrafficCommand

| OSI Message                                                      | Required / Optional |
|------------------------------------------------------------------|---------------------|
| `sensor_view.global_ground_truth.timestamp`                      | required            |
| `sensor_view.global_ground_truth.host_vehicle_id`                | required            |
| `sensor_view.global_ground_truth.moving_object.id`               | required            |
| `sensor_view.global_ground_truth.moving_object.base.position`    | required            |
| `sensor_view.global_ground_truth.moving_object.base.orientation` | required            |
| `sensor_view.global_ground_truth.moving_object.base.velocity`    | required            |
| `traffic_command.traffic_participant_id`                         | required            |
| `traffic_command.action`                                         | optional            |

### Output: Fields in osi3::TrafficUpdate and/or osi3::TrafficCommandUpdate filled by the Model

| OSI Message                               | Required / Optional |
|-------------------------------------------|---------------------|
| `traffic_update.version`                  | required            |
| `traffic_update.timestamp`                | required            |
| `traffic_update.update.base.position`     | required            |
| `traffic_update.update.base.orientation`  | required            |
| `traffic_update.update.base.velocity`     | required            |
| `traffic_command_update.version`          | required            |
| `traffic_command_update.timestamp`        | required            |
| `traffic_command_update.dismissed_action` | optional            |

## Build Instructions

What are the dependencies for building the model?

Give step-by-step build instructions for supported operating systems.
The following is an example for building a model as an FMU in Ubuntu.

### Build Model in Ubuntu 18.04 / 20.04

1. Clone this repository **with submodules**:

    ```bash
    git clone https://github.com/openMSL/your-model.git --recurse-submodules
    ```

2. Build the model by executing in the extracted project root directory:

    ```bash
    mkdir cmake-build
    cd cmake-build
    # If FMU_INSTALL_DIR is not set, CMAKE_BINARY_DIR is used
    cmake -DCMAKE_BUILD_TYPE=Release -DFMU_INSTALL_DIR:PATH=/tmp ..
    make
    ```

3. Take FMU from `FMU_INSTALL_DIR`

< The final FMU has to be named according to the repository name. In this example sl-1-0-sensor-model-repository-template.fmu >

## Credits

Give credits to funding or third-party contributions.
If the model is further described in a publication, it can also be named here.

## References

Throughout this readme file, references are to be used, e.g. when citing scientific literature while describing the modeling approach.
This can be done by added a number in the text with a reference identifier, like so: [[1](#Rosenberger2020)</sup>, p. 192].
For longer papers or book sections, please also give the page number, as shown in this example.

Then add the full list of authors, title and journal or conference in this section.
The IEEE citation style [[2](#IEEEStyle)</sup>] should be used.
Here is the bibliography from the example above.

<a name="Rosenberger2020">[1]</a>
P. Rosenberger, M. F. Holder, N. Cianciaruso, P. Aust, J. F. Tamm-Morschel, C. Linnhoff, and H. Winner,
“Sequential lidar sensor system simulation: A modular approach for simulation-based safety validation of automated driving,”
Automotive and Engine Technology, vol. 5, no. 3-4, pp. 187–197, Dec. 2020.

<a name="IEEEStyle">[2]</a>
IEEEDataPort,
"How to Cite References: IEEE Documentation Style,"
[Online]. Available:
[https://ieee-dataport.org/sites/default/files/analysis/27/IEEE%20Citation%20Guidelines.pdf](https://ieee-dataport.org/sites/default/files/analysis/27/IEEE%20Citation%20Guidelines.pdf).
[Accessed Jan. 09, 2023]

## State-of-the-Art

This section is a state-of-the-art collection for the traffic participant model sub-library.
When you clone this template to implement your own model, delete this entire section from the readme.

### Traffic Participant Model Validation and Verification

#### General Simulation Credibility

* [Credible Simulation Process Framework](https://gitlab.setlevel.de/open/processes_and_traceability/credible_simulation_process_framework) from the German research project [SET Level](https://setlevel.de/) of the [PEGASUS](https://pegasus-family.de/)  project family
* [Credibility-Assessment-Framework](https://github.com/virtual-vehicle/Credibility-Assessment-Framework) incl. [Credibility Development Kit](https://github.com/virtual-vehicle/Credibility-Assessment-Framework/tree/main/Credibility-Development-Kit)
from the European research project [UPSIM](https://upsim-project.eu/) incl. corresponding [publication](https://ecp.ep.liu.se/index.php/modelica/article/view/572/542)

##### Dedicated Publications

| Authors            | Date       | Title                            | Link / Repo / Paper / DOI                            | Data Set | Modality  | Facility  | Funding |
| ------------------ | ---------- | -------------------------------- | ---------------------------------------------------- | -------- | --------- | --------- | ------- |

### Collection of Traffic Participant Model Candidates for OpenMSL

#### Vehicle Models

| Authors       | Date       | Title                              | Link / Repo / Paper / DOI                            | Standards? | Facility  | Funding |
| ------------- | ---------- | ---------------------------------- | ---------------------------------------------------- | ---------- | --------- | ------- |

#### Pedestrian Models

| Authors           | Date       | Title                                                                                              | Link / Repo / Paper / DOI                                                    | Standards? | Facility  | Funding |
| ----------------- | ---------- | -------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------- | ---------- | --------- | ------- |
