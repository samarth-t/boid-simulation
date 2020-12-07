#include <core/boid.h>

namespace boidsimulation {

void Boid::Update(std::vector<Boid>& flock) {
  velocity_ += FlockingBehavior(flock);
  if(velocity_.Length() > max_speed_) {
    velocity_.ChangeMagnitude(max_speed_);
  }
  position_ += velocity_;
}

MathVector Boid::FlockingBehavior(std::vector<Boid>& flock) {
  MathVector flocking;
  flocking += (separation_scale_ * Separation(flock));
  flocking += (alignment_scale_ * Alignment(flock));
  flocking += (cohesion_scale_ * Cohesion(flock));
  return flocking;
}
MathVector Boid::Separation(std::vector<Boid>& flock) {
  MathVector separation;
  size_t count = 0;
  for(size_t boid_index = 0; boid_index < flock.size(); ++boid_index) {
    //checking if other Boid is visible to current Boid
    double distance = position_.Distance(flock.at(boid_index).position_);
    if(distance > 0 && distance <= 2.5 * size_) {
      MathVector difference = flock.at(boid_index).position_ - position_;
      separation -= difference;
      ++count;
    }
  }
  return separation;
}
MathVector Boid::Alignment(std::vector<Boid>& flock) {
  MathVector heading;
  size_t count = 0;
  for(size_t boid_index = 0; boid_index < flock.size(); ++boid_index) {
    //checking if other Boid is visible to current Boid
    double distance = position_.Distance(flock.at(boid_index).position_);
    if(distance > 0 && distance <= vision_) {
      heading += flock.at(boid_index).velocity_;
      ++count;
    }
  }
  //calculating average heading of flock
  if (count > 0) {
    heading /= count;
    //vector pointing from velocity to avg heading of visible flock
    MathVector alignment_force = (heading - velocity_) / 5;
    return alignment_force;
  } else {
    return heading;
  }
}
MathVector Boid::Cohesion(std::vector<Boid>& flock) {
  MathVector center;
  double count = 0;
  for(size_t boid_index = 0; boid_index < flock.size(); ++boid_index) {
    //checking if other Boid is visible to current Boid
    double distance = position_.Distance(flock.at(boid_index).position_);
    if(distance > 0 && distance <= vision_) {
      center += flock.at(boid_index).position_;
      ++count;
    }
  }
  //calculating average position of flock
  if(count > 0) {
    center /= count;
    //vector pointing from position to center of visible flock
    MathVector cohesion_force = (center - position_) / 25;
    return cohesion_force;
  } else {
    return center;
  }
}

void Boid::WallCollide(int axis) {
  if(axis == 0) {
    velocity_.x_ = -velocity_.x_;
  } else if (axis == 1) {
    velocity_.y_ = -velocity_.y_;
  } else if (axis == 2) {
    velocity_.z_ = -velocity_.z_;
  }
}

//Getters & Setters
const MathVector& Boid::GetPosition() const {
  return position_;
}
const MathVector& Boid::GetVelocity() const {
  return velocity_;
}
double Boid::GetSize() const {
  return size_;
}
const ci::Color8u& Boid::GetColor() const {
  return color_;
}
double Boid::GetSeparationScale() const {
  return separation_scale_;
}
double Boid::GetAlignmentScale() const {
  return alignment_scale_;
}
double Boid::GetCohesionScale() const {
  return cohesion_scale_;
}
void Boid::SetSeparationScale(double separationScale) {
  separation_scale_ = separationScale;
}
void Boid::SetAlignmentScale(double alignmentScale) {
  alignment_scale_ = alignmentScale;
}
void Boid::SetCohesionScale(double cohesionScale) {
  cohesion_scale_ = cohesionScale;
}

}  // namespace idealgas

