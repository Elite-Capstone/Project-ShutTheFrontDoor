/* Copyright 2018-2020 Google LLC
 *
 * This is part of the Google Cloud IoT Device SDK for Embedded C.
 * It is licensed under the BSD 3-Clause license; you may not use this file
 * except in compliance with the License.
 *
 * You may obtain a copy of the License at:
 *  https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __IOTC_MQTT_H__
#define __IOTC_MQTT_H__

#ifdef __cplusplus
extern "C" {
#endif

/*! \file
 * @brief The MQTT header flags.
 */

/**
 * @typedef iotc_mqtt_retain_t
 * @brief The MQTT
 * <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718030">
 * retain</a> flag.
 *
 * @see iotc_mqtt_retain_e
 */
typedef enum iotc_mqtt_retain_e {
  /** 0 */
  IOTC_MQTT_RETAIN_FALSE = 0,
  /** 1 */
  IOTC_MQTT_RETAIN_TRUE = 1,
} iotc_mqtt_retain_t;

/**
 * @typedef iotc_mqtt_qos_t
 * @brief The MQTT
 * <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718099">
 * Quality of Service</a> levels.
 *
 * @see iotc_mqtt_qos_e
 */
typedef enum iotc_mqtt_qos_e {
  /** 0 */
  IOTC_MQTT_QOS_AT_MOST_ONCE = 0,
  /** 1 */
  IOTC_MQTT_QOS_AT_LEAST_ONCE = 1,
  /** 2 */
  IOTC_MQTT_QOS_EXACTLY_ONCE = 2,
} iotc_mqtt_qos_t;

/**
 * @typedef iotc_mqtt_suback_status_t
 * @brief The MQTT
 * <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718071">
 * SUBACK payload</a>.
 *
 * @see iotc_mqtt_suback_status_e
 */
typedef enum iotc_mqtt_suback_status_e {
  /** 0x00 */
  IOTC_MQTT_QOS_0_GRANTED = 0x00,
  /** 0x01 */
  IOTC_MQTT_QOS_1_GRANTED = 0x01,
  /** 0x02 */
  IOTC_MQTT_QOS_2_GRANTED = 0x02,
  /** 0x80 */
  IOTC_MQTT_SUBACK_FAILED = 0x80
} iotc_mqtt_suback_status_t;

/**
 * @typedef iotc_mqtt_dup_t
 * @brief The MQTT
 * <a href="http://docs.oasis-open.org/mqtt/mqtt/v3.1.1/os/mqtt-v3.1.1-os.html#_Toc398718038">
 * DUP</a> flag.
 */
typedef enum iotc_mqtt_dup_e {
  /** 0 */
  IOTC_MQTT_DUP_FALSE = 0,
  /** 1 */
  IOTC_MQTT_DUP_TRUE = 1,
} iotc_mqtt_dup_t;

#ifdef __cplusplus
}
#endif

#endif /* __IOTC_MQTT_H__ */
